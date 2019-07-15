//
// Created by kirill on 26.02.19.
//

#include "plx9030c.h"

using namespace PLX9030;

plx9030::plx9030(std::string device) {
    fd = open(device.c_str(),O_RDWR);
    if(fd < 0){
        status = STATUS_FD_ERROR;
        return;
    }
    status = STATUS_OK;
}

plx9030::~plx9030() {
    close(fd);
}

int plx9030::getStatus() {
    return status;
}

int plx9030::getFileDescriptor() {
    return fd;
}

/* Function for write/read CS0..3 */
char plx9030::read8(unsigned long int base, long int offset) {
    unsigned char byte = 0x00;
    if(ioctl(fd,base,offset) < 0) status = STATUS_IOCTL_ERROR;
    if(read(fd,&byte,1) < 0) status = STATUS_READ_ERROR;
    return byte;
}

void plx9030::write8(unsigned long int base, long int offset, char byte) {
    if(ioctl(fd,base,offset) < 0) status = STATUS_IOCTL_ERROR;
    if(write(fd,&byte,1) <0 ) status = STATUS_WRITE_ERROR;
}

uint16_t plx9030::read16(unsigned long int base, long int offset) {
    uint16_t retval = 0;
    // 16 bit = 2 bytes
    for(int i=0;i<2;i++){
        retval |= ((read8(base,offset+i)&0xff) << 8*i);
    }
    return retval;
}

void plx9030::write16(unsigned long int base, long int offset, uint16_t word) {
    char byte = 0;
    for(int i=0;i<2;i++){
        byte = (char)((word>>8*i)&0xff);
        write8(base,offset+i,byte);
    }
}

uint32_t plx9030::read24(unsigned long int base, long int offset) {
    uint32_t retval = 0;
    // 24 bit = 3 bytes
    for(int i=0;i<3;i++){
        retval |= ((read8(base,offset+i)&0xff) << 8*i);
    }
    return retval;
}

void plx9030::write24(unsigned long int base, long int offset, uint32_t twoword) {
    char byte = 0;
    for(int i=0;i<3;i++){
        byte = (char)((twoword>>8*i)&0xff);
        write8(base,offset+i,byte);
    }
}

uint32_t plx9030::read32(unsigned long int base, long int offset) {
    uint32_t retval = 0;
    for(int i=0;i<4;i++){
        retval |= ((read8(base,offset+i)&0xff) << 8*i);
    }
    return retval;
}

void plx9030::write32(unsigned long int base, long int offset, uint32_t twoword) {
    char byte = 0;
    for(int i=0;i<4;i++){
        byte = (char)((twoword>>8*i)&0xff);
        write8(base,offset+i,byte);
    }
}

void plx9030::setMask(unsigned long int base, long int offset, unsigned char mask) {
    char byte = read8(base,offset);
    byte |= mask;
    write8(base,offset,byte);
}

void plx9030::unsetMask(unsigned long int base, long int offset, unsigned char mask) {
    char byte = read8(base,offset);
    byte &= ~mask;
    write8(base,offset,byte);
}