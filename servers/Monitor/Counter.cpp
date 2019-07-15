//
// Created by alex on 25.02.19.
//

#include "Counter.h"

Counter::Counter(std::string device) {
    /* set descriptor and open device */
    dev_desc = open(device.c_str(),O_RDWR);
    if(dev_desc < 0){
        status = COUNTER_STATUS_ERROR_FILE_DESCRIPTOR;
        return;
    }

    /* read 4 ofsset addr from cs0 */
    char test = (char)(read8CS0(COUNTER_TIMER_STATUS_REGISTER)&0x7f);  // <- this is value 0x55
    if(test != 0x55){
        status = COUNTER_STATUS_NOT_COUNTER_DEVICE; //this is not counter device!
        return;
    }

    write8CS0(COUNTER_CONTROL_REGISTER,0x00);
    write8CS0(COUNTER_TIMER_REGISTER,0x00);

}

Counter::~Counter() {

    close(dev_desc);
}

uint32_t Counter::readCounter(short channel) {
    return read32CS0(16+channel*4);
}

void Counter::writeCounter(uint32_t value, short channel) {
    write32CS0(16+channel*4, value);
}


void Counter::setTimeInterval(unsigned int time) {
    uint32_t buff_reg = 0xffffffff - time;
    writeTimer(buff_reg);
}

void Counter::startTimer() {
    write8CS0(COUNTER_CONTROL_REGISTER,0x00);
    /* stop and reset timer */
    char control_timer = read8CS0(COUNTER_TIMER_REGISTER);
    control_timer |= (COUNTER_TR_GATE_SEL | COUNTER_TR_CLR_GATE);
    write8CS0(COUNTER_TIMER_REGISTER,control_timer);

    /* load value from preset value register (buffer register) to timer value register (timer read register) */
    control_timer = read8CS0(COUNTER_TIMER_REGISTER);
    control_timer |= COUNTER_TR_LOAD_GATE;
    control_timer &=~ COUNTER_TR_CLR_GATE;
    write8CS0(COUNTER_TIMER_REGISTER,control_timer);


    write8CS0(COUNTER_CONTROL_REGISTER,0x0f);                    // write 0b1111 to D0..D3 in CONTROL REGISTER (operation "not" for all inputs)
    char control_register = read8CS0(COUNTER_CONTROL_REGISTER);
    control_register|= (COUNTER_CR_RGCLR|COUNTER_CR_END_CRM);
    write8CS0(COUNTER_CONTROL_REGISTER,control_register);

    control_timer = COUNTER_TR_GATE_SEL | COUNTER_TR_EN_GATE;     // start timer!
    write8CS0(COUNTER_TIMER_REGISTER,control_timer);
}

void Counter::stopTimer() {
    char control_timer = read8CS0(COUNTER_TIMER_REGISTER);
    control_timer &=~ (COUNTER_TR_EN_GATE|COUNTER_TR_CLR_GATE);
    write8CS0(COUNTER_TIMER_REGISTER,control_timer);

    char control_register = read8CS0(COUNTER_CONTROL_REGISTER);
    control_register|= COUNTER_CR_END_CRM;
    write8CS0(COUNTER_CONTROL_REGISTER,control_register);
}


bool Counter::testTimer() {
    /* test GATE_STATE bit */
    if(read8CS0(COUNTER_TIMER_STATUS_REGISTER)&0x80){
        return true;
    }else {
        return false;
    }
}

void Counter::resetCounter() {
    char control_register = read8CS0(COUNTER_CONTROL_REGISTER);
    control_register &= ~COUNTER_CR_RGCLR;  // clear counters
    write8CS0(COUNTER_CONTROL_REGISTER,control_register);
}

uint32_t Counter::readTimer(){
    uint32_t retval = read32CS0(COUNTER_TIMER_VALUE_REGISTER);
    return retval;
}

void Counter::writeTimer(uint32_t value) {
    write32CS0(COUNTER_TIMER_BUFFER_REGISTER,value);
}

void Counter::resetTimer() {
    char control_timer = read8CS0(COUNTER_TIMER_REGISTER);
    control_timer |= COUNTER_TR_CLR_GATE;
    write8CS0(COUNTER_TIMER_REGISTER,control_timer);
}


unsigned char Counter::read8CS0(long offset){
    unsigned char byte = 0x00;
    ioctl(dev_desc,CS0_SET_ADDR,offset);
    read(dev_desc,&byte,1);
    return byte;
}

int Counter::write8CS0(long offset, char byte){
    int retval;
    retval = ioctl(dev_desc,CS0_SET_ADDR,offset);
    retval = (int) write(dev_desc,&byte,1);
    return retval;
}

uint16_t Counter::read16CS0(long offset) {
    uint16_t retval = 0;
    retval = read8CS0(offset);
    retval |= (read8CS0(offset+1) << 8);
    return  retval;
}

int Counter::write16CS0(long offset, uint16_t word){
    int retval;
    char byte1 = (char)(word & 0x00ff);
    char byte2 = (char)((word >> 8) & 0x00ff);
    retval = write8CS0(offset,byte1);
    retval = write8CS0(offset+1,byte2);
    return retval;
}

uint32_t Counter::read32CS0(long offset) {
    uint32_t retval = 0;
    retval = read16CS0(offset);
    retval|= (read16CS0(offset+2) << 16);
    return retval;
}

int Counter::write32CS0(long offset, uint32_t data){
    int retval;
    uint16_t word1 = (uint16_t)(data & 0xffff);
    uint16_t word2 = (uint16_t)((data >> 16) & 0xffff);
    retval = write16CS0(offset, word1);
    retval = write16CS0(offset+2,word2);
    return retval;
}


unsigned char Counter::read8CS1(long offset){
    unsigned char byte = 0x00;
    ioctl(dev_desc,CS1_SET_ADDR,offset);
    read(dev_desc,&byte,1);
    return byte;
}

int Counter::write8CS1(long offset, char byte){
    int retval;
    retval = ioctl(dev_desc,CS1_SET_ADDR,offset);
    retval = (int) write(dev_desc,&byte,1);
    return retval;
}

uint16_t Counter::read16CS1(long offset) {
    uint16_t retval = 0;
    retval = read8CS1(offset);
    retval |= (read8CS1(offset+1) << 8);
    return  retval;
}

int Counter::write16CS1(long offset, uint16_t word){
    int retval;
    char byte1 = (char)(word & 0x00ff);
    char byte2 = (char)((word >> 8) & 0x00ff);
    retval = write8CS1(offset,byte1);
    retval = write8CS1(offset+1,byte2);
    return retval;
}

void Counter::setBitCS0(long offset, int numbit){
    char tmp_byte;
    tmp_byte = read8CS0(offset);
    tmp_byte |= (1<<numbit);
    write8CS0(offset,tmp_byte);
    return;
}

void Counter::unsetBitCS0(long offset, int numbit){
    char tmp_byte;
    tmp_byte = read8CS0(offset);
    tmp_byte &= ~(1<<numbit);
    write8CS0(offset,tmp_byte);
    return;
}