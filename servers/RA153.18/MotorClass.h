//
// Created by Kirill Pshenichnyi on 17.12.18.
//

#ifndef FOURCHANNELADAPTER_MOTORCLASS_H
#define FOURCHANNELADAPTER_MOTORCLASS_H

#define DEBUG_MESSAGE

#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <asm/ioctls.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <poll.h>
#include <boost/asio.hpp>

#ifdef DEBUG_MESSAGE
#include <stdio.h>
#endif


namespace Motor {
    /* FLAGS */
    const int F_MOTOR_POWER_ON                    = 0x20;
    const int F_MOTOR_DIRECTION                   = 0x40;
    const int F_MOTOR_ENABLE                      = 0x80;

    const int F_SENSOR_MOTORCONTROL               = 0x20;
    const int F_SENSOR_GRAYCODE                   = 0x40;
    const int F_SENSOR_ENABLE                     = 0x80;

    /* CMD */
    const int CMD_SENSOR_READ                     = 0x01;
    const int CMD_SENSOR_WCONFIG                  = 0x11;
    const int CMD_SENSOR_RCONFIG                  = 0x21;

    const int CMD_MOTOR_READ                      = 0x02;
    const int CMD_MOTOR_WCONFIG                   = 0x12;
    const int CMD_MOTOR_RCONFIG                   = 0x22;
    const int CMD_MOTOR_WRITE                     = 0x82;

    const int CMD_TASK_READ                       = 0x04;
    const int CMD_TASK_WCONFIG                    = 0x14;
    const int CMD_TASK_RCONFIG                    = 0x24;
    const int CMD_TASK_WRITE                      = 0x84;

    const int CMD_READALL                         = 0x00;
    const int CMD_ECHO                            = 0x60;

    const int CMD_GET_PARAM                       = 0x40;
    const int CMD_SET_PARAM                       = 0x43;
    const int CMD_SAVE_PARAMS                     = 0x44;
    const int CMD_RESTORE_PARAMS                  = 0x45;

    const int CMD_ERROR                           = 0x70;

    struct sReadAll{
        struct{
            uint32_t flags;
            uint32_t sensor_value;
            uint32_t sensor_stop_value;
            uint32_t steps_left;
        } channel[4];
    };

    /* for cmdSensorRead */
    struct sSensor{
        char channel;
        char flags;
        char nbytes_nbits;
        char nshift;
        uint32_t value;
    };

    /* cmdSensorWconfig, cmdSensorRconfig */
    struct sSensorConfig{
        char channel;
        char flags;
        char nbytes_nbits;
        char nshift;
        uint32_t mask;
    };

    /* for cmdMotorRead, cmdMotorWrite */
    struct sMotorRW{
        char channel;
        char flags;
        uint16_t freq_idx;
        uint32_t stepl_left;
    };

    /* for cmdMotorWconfig, cmdMotorRconfig */
    struct sMotorConfig{
        char channel;
        char flags;
        char accel;
        char stepping;
        uint32_t max_idx;
    };

    /* for cmdTaskWconfig, cmdTaskRconfig */
    struct sTaskConfig{
        char channel;
        char flags;
        uint16_t mot_spr;
        uint32_t sens_spr;
    };

    /* for cmdTaskRead */
    struct sTaskRead{
        char channel;
        char flags;             // flags for task
        char try_number;
        char flags_sm;          // flags for step motors
        uint32_t sensor_code;
    };

    /* for cmdTaskWrite */
    struct sTaskWrite{
        char channel;
        char direction;
        uint16_t try_number;
        uint32_t target_code;
    };

    struct sPacket{
        uint16_t title;
        char device_type;
        char num_of_device;
        char command;
        char data[8];
        char checksum;
        uint16_t signature;
    };

    struct motorStatus{
        bool leftEnd;
        bool rightEnd;
        uint32_t leftSteps;
        uint32_t sensorValue;
    };

    /* CLASS */
    class MotorClass {
    public:
        int channel;

    private:
        int device;
        boost::asio::serial_port *sp;

        struct sPacket rx_packet;
        struct sPacket tx_packet;

    private:
        char getCheckSum(char *buff);
        void setTitle(void);
        int rawWriteRead(char *in_buff,char *out_buff,int write_size,int read_size);
        int SendPacket();

        void pushData(sPacket *pack,char * data);
        void popData(sPacket *pack,char * data);
        void sendPushPop(char * rxdata,char * txdata);

    public:
        MotorClass(boost::asio::serial_port *serial_port);
        void setChannel(int channel_value);
        void setDevice(int device_value);

        char setNbitsNbytes(int nbits, int nbytes);

        int convertFromGrayCode(int value);

        void cmdSensorRead(struct sSensor *ssensor);
        void cmdSensorWconfig(struct sSensorConfig *ssensorconf);
        void cmdSensorRconfig(struct sSensorConfig *ssensorconf);

        void cmdMotorRead(sMotorRW *smotorrw);
        void cmdMotorWrite(sMotorRW *smotorrw);
        void cmdMotorRconfig(sMotorConfig *smotorconfig);
        void cmdMotorWconfig(sMotorConfig *smotorconfig);

        void cmdTaskRconfig(sTaskConfig *staskconfig);
        void cmdTaskWconfig(sTaskConfig *staskconfig);
        void cmdTaskWrite(sTaskWrite *staskwrite);
        void cmdTaskRead(sTaskRead *staskread);

        void moveToLeft(int steps);
        void moveToRight(int steps);
        void stopMotion();

        bool cmdEcho();

        struct motorStatus getMotorStatus(void);

    };
}


#endif //FOURCHANNELADAPTER_MOTORCLASS_H
