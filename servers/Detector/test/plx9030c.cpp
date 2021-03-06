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
char plx9030::read8(uint32_t base, long int offset) {
    unsigned char byte = 0x00;
    if(ioctl(fd,base,offset) < 0) status = STATUS_IOCTL_ERROR;
    if(read(fd,&byte,1) < 0) status = STATUS_READ_ERROR;
    return byte;
}

void plx9030::write8(uint32_t base, long int offset, char byte) {
#ifdef DEBUG
    switch (base){
        case CS0:
            std::cout << "CS0+" << std::dec << offset;
            break;
        case CS1:
            std::cout << "CS1+" << std::dec << offset;
            break;
        case CS2:
            std::cout << "CS2+" << std::dec << offset;
            break;
        case CS3:
            std::cout << "CS3+" << std::dec << offset;
            break;
    }

    std::cout << std::hex << " 0x" << (byte&0xff) << " -> " << "0x" << (read8(base,offset)&0xff);
#endif

    if(ioctl(fd,base,offset) < 0) status = STATUS_IOCTL_ERROR;
    if(write(fd,&byte,1) <0 ) status = STATUS_WRITE_ERROR;

#ifdef DEBUG
    //sleep(1);
    std::cout << " = 0x" << (read8(base,offset)&0xff) << std::dec << "\n";
#endif
}

uint16_t plx9030::read16(uint32_t base, long int offset) {
    uint16_t retval = 0;
    // 16 bit = 2 bytes
    for(int i=0;i<2;i++){
        retval |= ((read8(base,offset+i)&0xff) << 8*i);
    }
    return retval;
}

void plx9030::write16(uint32_t base, long int offset, uint16_t word) {
    char byte = 0;
    for(int i=0;i<2;i++){
        byte = (char)((word>>8*i)&0xff);
        write8(base,offset+i,byte);
    }
}

uint32_t plx9030::read24(uint32_t base, long int offset) {
    uint32_t retval = 0;
    // 24 bit = 3 bytes
    for(int i=0;i<3;i++){
        retval |= ((read8(base,offset+i)&0xff) << 8*i);
    }
    return retval;
}

void plx9030::write24(uint32_t base, long int offset, uint32_t twoword) {
    char byte = 0;
    for(int i=0;i<3;i++){
        byte = (char)((twoword>>8*i)&0xff);
        write8(base,offset+i,byte);
    }
}

uint32_t plx9030::read32(uint32_t base, long int offset) {
    uint32_t retval = 0;
    for(int i=0;i<4;i++){
        retval |= ((read8(base,offset+i)&0xff) << 8*i);
    }
    return retval;
}

void plx9030::write32(uint32_t base, long int offset, uint32_t twoword) {
    char byte = 0;
    for(int i=0;i<4;i++){
        byte = (char)((twoword>>8*i)&0xff);
        write8(base,offset+i,byte);
    }
}

void plx9030::setMask(uint32_t base, long int offset, unsigned char mask) {
    char byte = read8(base,offset);
    byte |= mask;
    write8(base,offset,byte);
}

void plx9030::unsetMask(uint32_t base, long int offset, unsigned char mask) {
    char byte = read8(base,offset);
    byte &= ~mask;
    write8(base,offset,byte);
}

void plx9030::readMap(int sizeCS0,int sizeCS1,int sizeCS2,int sizeCS3){
    char *cCS0 = new char[sizeCS0];
    char *cCS1 = new char[sizeCS1];
    char *cCS2 = new char[sizeCS2];
    char *cCS3 = new char[sizeCS3];

    for(int i=0;i<sizeCS0;i++){
        cCS0[i] = read8(CS0,i);
    }

    for(int i=0;i<sizeCS1;i++){
        cCS1[i] = read8(CS1,i);
    }

    for(int i=0;i<sizeCS2;i++){
        cCS2[i] = read8(CS2,i);
    }

    for(int i=0;i<sizeCS3;i++){
        cCS3[i] = read8(CS3,i);
    }

    std::cout << "---------------- CS0 ----------------------\n";
    for(int i=0;i<sizeCS0;i++){
        std::cout << "CS0+" << std::dec << i << "\t"
                  << "0x" << std::hex  << (cCS0[i]&0xff) << "\n";
    }

    std::cout << "---------------- CS1 ----------------------\n";
    for(int i=0;i<sizeCS1;i++){
        std::cout << "CS1+" << std::dec << i << "\t"
                  << "0x" << std::hex  << (cCS1[i]&0xff) << "\n";
    }

    std::cout << "---------------- CS2 ----------------------\n";
    for(int i=0;i<sizeCS2;i++){
        std::cout << "CS2+" << std::dec << i << "\t"
                  << "0x" << std::hex  << (cCS2[i]&0xff) << "\n";
    }

    std::cout << "---------------- CS3 ----------------------\n";
    for(int i=0;i<sizeCS3;i++){
        std::cout << "CS3+" << std::dec << i << "\t"
                  << "0x" << std::hex  << (cCS3[i]&0xff) << "\n";
    }

    delete [] cCS0;
    delete [] cCS1;
    delete [] cCS2;
    delete [] cCS3;

}
