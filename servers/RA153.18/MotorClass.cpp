//
// Created by Kirill Pshenichny on 17.12.18.
//

#include "MotorClass.h"

using namespace Motor;

MotorClass::MotorClass(boost::asio::serial_port *serial_port){
    sp = serial_port;
    setTitle();
}

void MotorClass::setChannel(int channel_value) {
    channel = channel_value;
}

void MotorClass::setDevice(int device_value) {
    device = device_value;
    setTitle();
}

int MotorClass::rawWriteRead(char *in_buff, char *out_buff, int write_size, int read_size) {
    int i = 0;
    char tmp[16];

#ifdef DEBUG_MESSAGE
    printf("Send: ");
#endif
    for (i = 0; i < write_size; i++) {
        tmp[i] = in_buff[i];
#ifdef DEBUG_MESSAGE
        if(i==5 || i==13) printf("  ");
        printf("0x%.2x ", tmp[i] & 0xff);
#endif
    }
#ifdef DEBUG_MESSAGE
    printf("\n");
#endif

    sp->write_some(boost::asio::buffer(tmp));
    int len = sp->read_some(boost::asio::buffer(tmp));

#ifdef DEBUG_MESSAGE
    printf("Recv: ");
#endif
    for(i=0;i<read_size;i++){
        out_buff[i] = tmp[i];
#ifdef DEBUG_MESSAGE
        if(i==5 || i==13) printf("  ");
        printf("0x%.2x ",out_buff[i] & 0xff);
#endif
    }
#ifdef DEBUG_MESSAGE
    printf("\n");
#endif

    return len;
}

void MotorClass::setTitle() {
    tx_packet.title = 0x5aa5;
    tx_packet.num_of_device = (char) (device & 0xff);
    tx_packet.device_type = 253;
    tx_packet.signature = 0x9bb9;
    return;
}

char MotorClass::getCheckSum(char *buff){
    char check = 0x00;
    for(int i=2;i<13;i++) check ^= buff[i];
    return check;
}

int MotorClass::SendPacket() {
    tx_packet.checksum = getCheckSum((char *) &tx_packet);
    rawWriteRead((char *) &tx_packet, (char *) &rx_packet,
                 sizeof(tx_packet), sizeof(rx_packet));
    return 0;
}

void MotorClass::pushData(Motor::sPacket *pack, char *data) {
    for(int i=0;i<8;i++) pack->data[i] = data[i];
}

void MotorClass::popData(Motor::sPacket *pack, char *data) {
    for(int i=0;i<8;i++) data[i] = pack->data[i];
}

void MotorClass::sendPushPop(char *rxdata, char *txdata) {
    pushData(&tx_packet,(char *)txdata);
    SendPacket();
    popData(&rx_packet,(char *)rxdata);
}

char MotorClass::setNbitsNbytes(int nbits, int nbytes) {
    return (char) (((nbytes-1) << 6)&0xc0)|(nbits & 0xff);
}

void MotorClass::cmdSensorRead(Motor::sSensor *ssensor) {
    tx_packet.command = CMD_SENSOR_READ;
    char *bytes;
    bytes = (char *) ssensor;
    for(int i=1;i<sizeof(sSensorConfig);i++) bytes[i] = 0x00;
    sendPushPop((char *) ssensor,(char *) ssensor);
}

void MotorClass::cmdSensorWconfig(Motor::sSensorConfig *ssensorconf) {
    tx_packet.command = CMD_SENSOR_WCONFIG;
    sendPushPop((char *) ssensorconf,(char *) ssensorconf);
}

void MotorClass::cmdSensorRconfig(Motor::sSensorConfig *ssensorconf) {
    tx_packet.command = CMD_SENSOR_RCONFIG;
    char *bytes;
    bytes = (char *) ssensorconf;
    for(int i=1;i<sizeof(sSensorConfig);i++) bytes[i] = 0x00;
    sendPushPop((char *) ssensorconf,(char *) ssensorconf);
}

void MotorClass::cmdMotorRead(Motor::sMotorRW *smotorrw) {
    tx_packet.command = CMD_MOTOR_READ;
    char *bytes;
    bytes = (char *) smotorrw;
    for(int i=1;i<sizeof(sSensorConfig);i++) bytes[i] = 0x00;
    sendPushPop((char *) smotorrw,(char *) smotorrw);
}

void MotorClass::cmdMotorWrite(Motor::sMotorRW *smotorrw) {
    tx_packet.command = CMD_MOTOR_WRITE;
    sendPushPop((char *) smotorrw,(char *) smotorrw);
}

void MotorClass::cmdMotorWconfig(Motor::sMotorConfig *smotorconfig) {
    tx_packet.command = CMD_MOTOR_WCONFIG;
    sendPushPop((char *) smotorconfig,(char *) smotorconfig);
}

void MotorClass::cmdMotorRconfig(Motor::sMotorConfig *smotorconfig) {
    tx_packet.command = CMD_MOTOR_RCONFIG;
    char *bytes;
    bytes = (char *) smotorconfig;
    for(int i=1;i<sizeof(sSensorConfig);i++) bytes[i] = 0x00;
    sendPushPop((char *) smotorconfig,(char *) smotorconfig);
}

void MotorClass::cmdTaskRconfig(Motor::sTaskConfig *staskconfig) {
    tx_packet.command = CMD_TASK_RCONFIG;
    char *bytes;
    bytes = (char *) staskconfig;
    for(int i=1;i<sizeof(sTaskConfig);i++) bytes[i] = 0x00;                 // all zero except channel
    sendPushPop((char *) staskconfig,(char *) staskconfig);
}

void MotorClass::cmdTaskWconfig(Motor::sTaskConfig *staskconfig) {
    tx_packet.command = CMD_TASK_WCONFIG;
    sendPushPop((char *) staskconfig,(char *) staskconfig);
}

void MotorClass::cmdTaskRead(Motor::sTaskRead *staskread) {
    tx_packet.command = CMD_TASK_READ;
    char *bytes;
    bytes = (char *) staskread;
    for(int i=1;i<sizeof(sTaskRead);i++) bytes[i] = 0x00;                 // all zero except channel
    sendPushPop((char *) staskread,(char *) staskread);
}

void MotorClass::cmdTaskWrite(Motor::sTaskWrite *staskwrite) {
    tx_packet.command = CMD_TASK_WRITE;
    sendPushPop((char *) staskwrite,(char *) staskwrite);
}

bool MotorClass::cmdEcho() {
    tx_packet.command = CMD_ECHO;
    SendPacket();

    char *rx,*tx;
    tx = (char *)&tx_packet;
    rx = (char *)&rx_packet;

    for(int i=0;i<16;i++) if(tx[i]!=rx[i]) return false;
    return true;
}

int MotorClass::convertFromGrayCode(int value) {
    int retval = 0;
    for (retval=0;value;){
        retval^=value;
        value=value>>1;
    }
    return retval;
}


void MotorClass::stopMotion() {
    Motor::sMotorRW s_motor;
    s_motor.channel = (char) (channel & 0xff);
    s_motor.flags = 0x00;			// clean all the flags
    s_motor.stepl_left = 0x00;		// zero steps
    cmdMotorWrite(&s_motor);
}

void MotorClass::moveToLeft(int steps) {
    Motor::sMotorRW s_motor;
    s_motor.channel = (char) (channel & 0xff);
    s_motor.flags = (char) (Motor::F_MOTOR_ENABLE | Motor::F_MOTOR_POWER_ON);
    s_motor.stepl_left = steps;
    cmdMotorWrite(&s_motor);
}

void MotorClass::moveToRight(int steps) {
    Motor::sMotorRW s_motor;
    s_motor.channel = (char) (channel & 0xff);
    s_motor.flags = (char) (Motor::F_MOTOR_ENABLE | Motor::F_MOTOR_POWER_ON | Motor::F_MOTOR_DIRECTION);
    s_motor.stepl_left = steps;
    cmdMotorWrite(&s_motor);
}

struct motorStatus MotorClass::getMotorStatus() {
    motorStatus ret;

    Motor::sMotorRW srx_motor;
    Motor::sSensor s_sensor,srx_sensor;
    srx_motor.channel = (char) (channel & 0xff);
    s_sensor.channel = (char) (channel & 0xff);
    cmdMotorRead(&srx_motor);

    srx_sensor = s_sensor;
    cmdSensorRead(&srx_sensor);

    ret.leftSteps = srx_motor.stepl_left;
    ret.leftEnd = false;
    ret.rightEnd = false;
    if(srx_motor.flags & 0x01) ret.leftEnd = true;
    if(srx_motor.flags & 0x02) ret.rightEnd = true;

    ret.sensorValue = srx_sensor.value;

    return ret;
}