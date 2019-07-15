//
// Created by kirill on 15.02.19.
//

#ifndef RA153_16_PNEUMATICS_H
#define RA153_16_PNEUMATICS_H

#include <cstdint>
#include <iostream>

#include "ControllerRA153_16.h"
#include "SerialPort.h"

#define VALVE_COUNT 16

class Pneumatics : private CTRL_RA_153_16::ControllerRA153_16 {
private:
    bool states[VALVE_COUNT];
public:
    explicit  Pneumatics(SP::SerialPort *sp, std::string addr);
    ~Pneumatics();

    /* set state (on or off) */
    void setValve(int number, bool stat);

    /* get power state (power ON or power OFF) */
    bool getState(int number);

private:
    /* write data to pneumo registers [D0..D16] two bytes, 1 bit - state 1 valve */
    void setRegister(uint16_t reg);
    /* read data from registers */
    uint16_t getRegister(void);
};


#endif //RA153_16_PNEUMATICS_H
