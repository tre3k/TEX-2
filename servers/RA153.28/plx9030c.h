#ifndef PLX9030C_H
#define PLX9030C_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stropts.h>

namespace PLX9030{

#define CS0_SET_ADDR 0xfacecaf0
#define CS1_SET_ADDR CS0_SET_ADDR+1
#define CS2_SET_ADDR CS0_SET_ADDR+2
#define CS3_SET_ADDR CS0_SET_ADDR+3

#define CS0 CS0_SET_ADDR
#define CS1 CS1_SET_ADDR
#define CS2 CS2_SET_ADDR
#define CS3 CS3_SET_ADDR


    enum{
        STATUS_OK,
        STATUS_FD_ERROR,
        STATUS_WRITE_ERROR,
        STATUS_READ_ERROR,
        STATUS_IOCTL_ERROR
    };

    class plx9030{

    private:
        /* this file descriptor of device */
        int fd;
        int status;

    public:
        explicit plx9030(std::string device);
        ~plx9030();

        /* return status, errors, etc. */
        int getStatus(void);
        /* just return fd variable */
        int getFileDescriptor(void);

        /* Functions for write/read CS0..3 for 8,16,24 and 32 bits */
        // base - CS0,CS1,CS2,CS3
        char read8(unsigned long int base, long int offset);
        void write8(unsigned long int base, long int offset, char byte);

        uint16_t read16(unsigned long int base, long int offset);
        void write16(unsigned long int base, long int offset, uint16_t word);

        uint32_t read24(unsigned long int base, long int offset);
        void write24(unsigned long int base, long int offset, uint32_t twoword);

        uint32_t read32(unsigned long int base, long int offset);
        void write32(unsigned long int base, long int offset, uint32_t twoword);

        /* set mask / unset mask - selective set/unset bits in register by mask */
        // mask - just or operation (bit1|bit2|bit3)
        void setMask(unsigned long int base, long int offset, unsigned char mask);
        void unsetMask(unsigned long int base, long int offset, unsigned char mask);
    };
}


#endif //PLX9030C_H

/* EOF */
