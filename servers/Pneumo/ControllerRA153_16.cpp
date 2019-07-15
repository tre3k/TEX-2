//
// Created by kirill on 15.02.19.
//

#include "ControllerRA153_16.h"

using namespace CTRL_RA_153_16;

ControllerRA153_16::ControllerRA153_16(SP::SerialPort *sport, std::string addr){
    sp = sport;
    setAddr(addr);
};

ControllerRA153_16::~ControllerRA153_16() {

}

void ControllerRA153_16::setAddr(std::string DeviceAddress) {
    short k = 2;
    for(int i=0;i<3;i++) command_packet.addr[i] = '0';
    /* just clone data from string DeviceAddress to struct */

    for(long i=DeviceAddress.length()-1;i>=0;i--){
        command_packet.addr[k] = DeviceAddress.c_str()[i];
        k--; if(k<0) break;
    }
}

void ControllerRA153_16::SendCommand(CTRL_RA_153_16::ControllerRA153_16::s_command *cmd, char *recv, size_t recv_size) {
    size_t send_size = 8;
    sp->readwrite((char *)cmd, recv, &send_size, &recv_size);
}

void ControllerRA153_16::setFreq(u_int16_t freq) {
    /* Set frequency */
    command_packet.code = 'L';
    command_packet.data[0] = (char)(freq & 0xff);
    command_packet.data[1] = (char)((freq & 0xff00) >> 8);
    command_packet.data[2] = '0'; command_packet.data[3] = '1';
    SendCommand(&command_packet);
}

void ControllerRA153_16::enableFreq(){
    /* Enable frequency */
    // example of packet : 081T4012, '081' - addr, 'T' - code
    command_packet.code = 'T';
    strcpy(command_packet.data,"4012");
    SendCommand(&command_packet);
}

void ControllerRA153_16::disableFreq() {
    command_packet.code = 'T';
    strcpy(command_packet.data,"0000");
    SendCommand(&command_packet);
}
