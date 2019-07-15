//
// Created by kirill on 15.02.19.
//

#ifndef RA153_16_SERIALPORT_H
#define RA153_16_SERIALPORT_H

#include <cstdint>
#include <time.h>
#include <boost/asio.hpp>

namespace SP {
    /* class SerialPort using boost:serial_port */
    class SerialPort {

    private:
        boost::asio::io_service ios;
        boost::asio::serial_port *sp;

        unsigned int baud_rate = 9600;           // default speed value

    public:
        /* Just contructor, path = /dev/tty* */
        explicit SerialPort(const char *path);
        ~SerialPort();

        /* method for readwrite to controller */
        void readwrite(char *req, char *res, size_t *size_req, size_t *size_res);

        /* methods for set and get private baud_rate */
        void setBaudRate(int br);
        int getBaudRate(void);

        /* method for close port */
        void Close(void);

    };
}


#endif //SERIALPORT_H
