//
// Created by alex on 04.03.19.
//

#include "controller_RA153_28.h"

using namespace cRA153_28;

controller_RA153_28::controller_RA153_28(PLX9030::plx9030 *p9030) {
    dev = p9030;
}

controller_RA153_28::~controller_RA153_28() {

}

void controller_RA153_28::setChannel(int channel) {
    c_channel = channel;
}
int controller_RA153_28::getChannel() { return  c_channel;}

void controller_RA153_28::setSpeed(uint32_t speed) {
    dev->write24(PLX9030::CS0, SPEED_REGISTER+c_channel*16, c_ssi_speed*1000000/(speed*2));
    // c_ssi_speed * 10000000 ??? или это что-то другое?
    c_speed = speed;
}
uint32_t controller_RA153_28::getSpeed() { return c_speed;}


void controller_RA153_28::setSSISpeed(uint16_t speed) {
    dev->write8(PLX9030::CS0, SSI_REGISTER, 0x00);
    dev->setMask(PLX9030::CS0, SSI_REGISTER, 0x01);             // up 0-bit

    dev->write16(PLX9030::CS0, SSI_SPEED_REGISTER, speed);
    c_ssi_speed = speed;
}
uint16_t controller_RA153_28::getSSISpeed() { return c_ssi_speed;}


void controller_RA153_28::initMotion() {
    uint32_t encoder_mask = 0;

    dev->write8(PLX9030::CS0, SSI_REGISTER, 0x00);
    dev->setMask(PLX9030::CS0, SSI_REGISTER, 0x01);             // up 0-bit

    dev->write16(PLX9030::CS0, SSI_SPEED_REGISTER, 50);

    // port 3 ENFSSI=1; CLRFSSI=1; что это?
    dev->write8(PLX9030::CS0, SSI_REGISTER, 0x03);

    // just set null registers
    dev->write8(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, 0x00);        // port 1
    dev->write8(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, 0x00);           // port 0

    dev->setMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, CLR_SSI);

    // set encoder bit
    dev->write8(PLX9030::CS0, ENCODER_BITS_REG+c_channel*16, encoder_bits+1);
    for(int i=0;i<encoder_bits;i++) encoder_mask |= (1<<i);
    dev->write32(PLX9030::CS1, ENCODER_MASK+c_channel*16, encoder_mask);

    dev->setMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, EN_SSI);        //set bit ENABLE SSI
    //dev->setMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, EN_ENDS);    //set 2-bit Enable ENDS
    dev->setMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, (1<<6));    //set 2-bit Enable ENDS

    dev->unsetMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, CYCLE_END|(1<<5)|(1<<2));   // сброс триггеров
}

void controller_RA153_28::runMition(uint32_t count_steps, bool direction) {
    // Enable frequency
    dev->setMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, CLR_FS0);
    // Enable steps
    dev->unsetMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, CLR_STEP0);
    // Set speed
    dev->write24(PLX9030::CS0, SPEED_REGISTER+c_channel*16, (uint32_t)(50000000/(c_speed*2)));


    // Enable write step counter
    dev->setMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, (1<<6));
    // Write step counts
    dev->write24(PLX9030::CS0, STEPS_REGISTER+c_channel*16, count_steps);
    // Set cycle end
    dev->setMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, CYCLE_END);

    if(direction){
        dev->setMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, DIR);
    }else{
        dev->unsetMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, DIR);
    }

    dev->setMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, (1<<2));  //turned ??!
    dev->setMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, (1<<6));  //disable ends?

    /* Enable Frequency and enable steps (not clean) */
    dev->setMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, ENF_FS0|CLR_STEP0);

    /* Power on and enable steps */
    dev->setMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, EN_STEP0);
    dev->setMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, PWR_ON);
}

void controller_RA153_28::stopMotion() {
    dev->unsetMask(PLX9030::CS0, CONTROL_SSI_AND_MOTION+c_channel*16, EN_STEP0);
    dev->unsetMask(PLX9030::CS0, PARAMETERS_SSI_AND_MOTION+c_channel*16, PWR_ON);
}

uint8_t controller_RA153_28::getEnds() {
    return (0x78&(dev->read8(PLX9030::CS0, ENDS_REG+c_channel*16)));
}

bool controller_RA153_28::getCentralEnd() {
    return getEnds() & CENTRAL_END ? false : true;
}

bool controller_RA153_28::getLeftEnd() {
    return getEnds() & LEFT_END ? false : true;
}

bool controller_RA153_28::getRightEnd() {
    return getEnds() & RIGHT_END ? false : true;
}

uint32_t controller_RA153_28::getCounts() {
    return dev->read24(PLX9030::CS0, STEPS_GET_REGISTER+c_channel*16);
}