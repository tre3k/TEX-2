//
// Created by kirill on 15.02.19.
//

#include "Pneumatics.h"

Pneumatics::Pneumatics(SP::SerialPort *sp, std::string addr) :
CTRL_RA_153_16::ControllerRA153_16(sp, addr) {
    for(int i=0;i<VALVE_COUNT;i++){
        states[i] = false;
    }
    setFreq(1000);
    disableFreq();

    uint16_t test = 0x8011;

    setRegister(test);
    std::cout << "register: " << std::hex << (getRegister()&0xffff) << "\n";

    enableFreq();

}

Pneumatics::~Pneumatics() {

}

void Pneumatics::setValve(int number, bool stat) {
    uint16_t reg = 0;
    reg = getRegister();
    if(stat) reg|= (1 << number);
    else     reg&=~(1 << number);
    setRegister(reg);

    if(stat){
        // on valve
        enableFreq();
        states[number] = true;
    }else{
        // off valve
        disableFreq();
        states[number] = false;
    }
}

bool Pneumatics::getState(int number) {
    /* command for get state from device */
    uint16_t reg = getRegister();
    bool state;
    for(int i=0;i<VALVE_COUNT;i++){
        state = false;
        if(reg & (1<<i)) state = true;
        if(states[i] != state) states[i] = state;
    }

    return states[number];
}

void Pneumatics::setRegister(uint16_t reg) {
    command_packet.code = 'N';
    command_packet.data[0] = (char)(reg & 0xff);
    command_packet.data[1] = (char)((reg & 0xff00) >> 8);
    command_packet.data[2] = '0'; command_packet.data[3] = '1';
    SendCommand(&command_packet);
}

uint16_t Pneumatics::getRegister() {
    uint16_t retvalue = 0;
    char buffer[4];
    bzero(buffer,4);
    command_packet.code = 'R';
    strcpy(command_packet.data,"0000");
    // Send command and get data to buffer from device
    SendCommand(&command_packet,buffer,4);
    retvalue = (uint16_t)(buffer[0] & 0xff);
    retvalue|= (uint16_t)((buffer[1] & 0xff) << 8);
    return retvalue;
}
