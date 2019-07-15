//
// Created by alex on 25.02.19.
//

#ifndef MONITOR_COUNTER_H
#define MONITOR_COUNTER_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stropts.h>

#include "plx9030c.h"


#define COUNTER_STATUS_OK 0
#define COUNTER_STATUS_ERROR_FILE_DESCRIPTOR -1
#define COUNTER_STATUS_NOT_COUNTER_DEVICE -2

/* Control Register Bits */
#define COUNTER_CONTROL_REGISTER 2
#define COUNTER_CR_RG_GATE 0x80
#define COUNTER_CR_LOAD_CRM 0x40
#define COUNTER_CR_END_CRM 0x20
#define COUNTER_CR_RGCLR 0x10

/* Time control register bits */
#define COUNTER_TIMER_REGISTER 3
#define COUNTER_TR_GATE_SEL 0x80
#define COUNTER_TR_LOAD_GATE 0x40
#define COUNTER_TR_EN_GATE 0x20
#define COUNTER_TR_CLR_GATE 0x10

/* timer value registers */
#define COUNTER_TIMER_STATUS_REGISTER 4
#define COUNTER_TIMER_BUFFER_REGISTER 8
#define COUNTER_TIMER_VALUE_REGISTER 12


class Counter {
public:
    int status = 0;
private:
    int dev_desc;                   // device descriptor

public:
    /* just constructor */
    explicit Counter(std::string device);
    ~Counter();

    /* set time interval in ms */
    void setTimeInterval(unsigned int time);

    /* functions for read and write to counter registers */
    uint32_t readCounter(short int channel = 0);
    void writeCounter(uint32_t value, short int channel = 0);

    /* function for read and write to timer */
    uint32_t readTimer(void);
    void writeTimer(uint32_t value);


    /* starttimer */
    void startTimer(void);
    /* stop Timer */
    void stopTimer(void);
    /* timer is end? */
    bool testTimer(void);
    /* reset Timer */
    void resetTimer(void);

    /* reset Counter */
    void resetCounter(void);




private:
    /* PCI Device function for read and write from and to CSx */
    /* read and write 8 bits */
    unsigned char read8CS0(long offset);
    int write8CS0(long offset, char byte);

    /* read and write 16 bits */
    uint16_t read16CS0(long offset);
    int write16CS0(long offset, uint16_t word);

    uint32_t read32CS0(long offset);
    int write32CS0(long offset, uint32_t data);

    /* also for CS1 */
    unsigned char read8CS1(long offset);
    int write8CS1(long offset, char byte);
    uint16_t read16CS1(long offset);
    int write16CS1(long offset, uint16_t word);

    /* function for set the selective bit on CS0 region */
    void setBitCS0(long offset, int numbit);
    void unsetBitCS0(long offset, int numbit);


};


#endif //MONITOR_COUNTER_H
