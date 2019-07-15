//
// Created by alex on 04.03.19.
//

#ifndef RA153_28_CONTROLLER_RA153_28_H
#define RA153_28_CONTROLLER_RA153_28_H

#include "plx9030c.h"

namespace cRA153_28 {

    /* CS0 */
    #define CONTROL_SSI_AND_MOTION 0x00
    #define CLR_FS0 0x01                                // 0-bit set/unset frequency formier
    #define ENF_FS0 0x02                                // 1-bit enable/disable frequency formier
    #define CLR_STEP0 0x04                              // 2-bit enable/clear steps
    #define EN_STEP0 0x08                               // 3-bit enable/disable steps
    #define CLR_SSI 0x10                                // 4-bit
    #define EN_SSI 0x20                                 // 5-bit

    #define PARAMETERS_SSI_AND_MOTION 0x01
    #define CYCLE_END 0x01                              // 0-bit trigger cycle end
    #define DIR 0x02                                    // 1-bit direction
    #define EN_ENDS 0x04                                // 2-bit enable ends (концевики)
    #define PWR_ON 0x08                                 // 3-bit power on

    #define SPEED_REGISTER 0x04

    #define STEPS_REGISTER 0x07
    #define STEPS_GET_REGISTER 0x0a

    #define ENCODER_BITS_REG 0x0d                       // (offset: 13)

    #define ENDS_REG 0x0f                               // (offset: 15) I'dnt no what is the register?! just experemntal.. =)
    #define LEFT_END 0x08                               // 3 bit
    #define CENTRAL_END 0x10                            // 4 bit
    #define RIGHT_END 0x20                              // 5 bit

    /* without channels */
    #define SSI_REGISTER 0x03                           // может называться по-другому...
    #define SSI_SPEED_REGISTER 0x13                     // (offset: 19)


    /* CS1 */
    #define ENCODER_MASK 0x04


    class controller_RA153_28 {
    /*global class private variable */
    private:
        PLX9030::plx9030 *dev;              // device

        int c_channel;
        uint32_t c_speed;
        uint16_t c_ssi_speed;


        uint8_t getEnds(void);
    /* global class public variable */
    public:
        uint8_t encoder_bits;

    /* Contructor */
    public:
        explicit controller_RA153_28(PLX9030::plx9030 *p9030);
        ~controller_RA153_28();

    public:
        void setChannel(int channel);
        int getChannel(void);

        /* Set and get motion speed */
        void setSpeed(uint32_t speed);
        uint32_t getSpeed(void);

        /* Set and get SSI Speed */
        void setSSISpeed(uint16_t speed);
        uint16_t getSSISpeed(void);

        /* init motion */
        void initMotion(void);
        /* run motion */
        void runMition(uint32_t count_steps, bool direction);
        /* stop motion */
        void stopMotion(void);
        /* central End */
        bool getCentralEnd(void);
        /* left End */
        bool getLeftEnd(void);
        /* right End */
        bool getRightEnd(void);

        /* read count steps */
        uint32_t getCounts(void);

    };

}

#endif //RA153_28_CONTROLLER_RA153_28_H
