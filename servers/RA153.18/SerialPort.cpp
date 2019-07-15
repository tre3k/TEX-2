//
// Created by kirill on 17.12.18.
//


#include "SerialPort.h"

using namespace SP;

SerialPort::SerialPort(const char *path) {

    sp = new boost::asio::serial_port(ios,path);
    sp->set_option(boost::asio::serial_port::baud_rate(9600));
#ifdef DEBUG_MESSAGE
    printf("PATH: %s\n",path);
#endif

}

SerialPort::~SerialPort() {
    Close();
}

void SerialPort::Close() {
    sp->close();
    delete sp;
}

void SerialPort::setSpeed(int value) {

}

/*
boost::asio::serial_port *SerialPort::getDescriptor() {
    return sp;
}
 */

int SerialPort::getSpeed() {
    return speed;
}