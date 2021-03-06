#include "tdc.h"

using namespace TDC;

tdc::tdc(std::string devname){
    plx = new plx9030(devname);
    init();
}

tdc::~tdc(){
    delete plx;
}

void tdc::init(){

    /*
    // Disable start and stop init TDC,reset FIFO
    plx->write8(CS0,0x00,0x00);
    // Reset TDC, RESET FIFO
    plx->write8(CS0,0x02,0x00);
    // Reset TDC,
    plx->write16(CS3,31*2,0);

    plx->write8(CS0,2,0x40);
    plx->write16(CS3,31*2,0x03);
                                          // short int         addr
    plx->write32(CS3,0*2,0x0007fc81);     // memCS3[1,0]       [3,2,1,0]
    plx->write32(CS3,2*2,0x00000000);     // memCS3[3,2]       [7,6,5,4]
    plx->write32(CS3,4*2,0x00000002);     // memCS3[5,4]       [11,10,9,8]
    plx->write32(CS3,6*2,0x00000000);     // memCS3[7,6]       [15,14,13,12]
    plx->write32(CS3,8*2,0x06000000);     // memCS3[9,8]       [19,18,17,16]
    plx->write32(CS3,10*2,0x00e00000);    // memCS3[11,10]     [23,22,21,20]
    plx->write32(CS3,12*2,0x00000000);    // memCS3[13,12]     [27,26,25,24]
    plx->write32(CS3,14*2,0x00141fb4);    // memCS3[15,14]     [31,30,29,28]
    plx->write32(CS3,22*2,0x07ff0000);    // memCS3[23,22]     [47,46,45,44]
    plx->write32(CS3,24*2,0x02000000);    // memCS3[23,22]     [51,50,49,48]
    // ... skip 32 bits ... //
    plx->write32(CS3,28*2,0x00000000);    // memCS3[23,22]     [59,58,57,56]

    plx->write32(CS3,8*2,0x06400000);     // Master reset

    //std::cout << "MEM: " << std::hex << "0x" << (plx->read16(CS3,30*2)&0xffff) << "\n";
    //plx->readMap(32,0,0,256);
     */

    plx->write8(CS0,0,0);
    plx->write8(CS0,2,0);
    plx->write16(CS3,31*2,0);
    plx->write8(CS0,2,0x40);
    plx->write16(CS3,31*2,3);

    // INIT

    plx->write16(CS3,1*2,0x007);        // BMemcs3[1]=0x007; //rising edges, Start ringoscil.
    plx->write16(CS3,0*2,0xfc81);       // BMemcs3[0]=0xfc81;

    plx->write16(CS3,3*2,0);            // BMemcs3[3]=0;
    plx->write16(CS3,2*2,0);            // BMemcs3[2]=0;

    plx->write16(CS3,5*2,0);            // BMemcs3[5]=0;//I-mode
    plx->write16(CS3,4*2,2);            // BMemcs3[4]=2;

    plx->write16(CS3,7*2,0);            // BMemcs3[7]=0;
    plx->write16(CS3,6*2,0);            // BMemcs3[6]=0;

    plx->write16(CS3,9*2,0x600);        // BMemcs3[9]=0x600;//Mtimes trig, by Sart, EFlagHiZN
    plx->write16(CS3,8*2,0);            // BMemcs3[8]=0;

    plx->write16(CS3,11*2,0x0E0);       // BMemcs3[11]=0x0E0;  //StopDisStart, StartOff1 = 0, MasterAluTrig
    plx->write16(CS3,10*2,0);          	// BMemcs3[10]=0x0;

    plx->write16(CS3,13*2,0);           // BMemcs3[13]=0;
    plx->write16(CS3,12*2,0);           // BMemcs3[12]=0;

    plx->write16(CS3,15*2,0x014);       // BMemcs3[15]=0x014; // 14 time meas = 1 mksec
    plx->write16(CS3,14*2,0x1FB4);      // BMemcs3[14]=0x1FB4; // Res = 82,3045 ps

    plx->write16(CS3,23*2,0x7FF);       // BMemcs3[23]=0x7FF; //Any error -> ErrFlag
    plx->write16(CS3,22*2,0);           // BMemcs3[22]=0;

    plx->write16(CS3,25*2,0x200);       // BMemcs3[25]=0x200; //Mtimer -> IrFlag
    plx->write16(CS3,24*2,0);           // BMemcs3[24]=0;

    plx->write16(CS3,29*2,0);           // BMemcs3[29]=0;
    plx->write16(CS3,28*2,0);           // BMemcs3[28]=0;

    plx->write16(CS3,9*2,0x640);        // BMemcs3[9]=0x640; // Master reset
    plx->write16(CS3,8*2,0);            // BMemcs3[8]=0;

    return;
}

void tdc::start(){
#ifdef DEBUG
    std::cout << "START\n";
#endif

    plx->write8(CS0,0,0x80); // D7 = 1, enable INTR
    plx->write8(CS0,2,0x40); // enable TDC

    plx->write16(CS3,31*2,0xf001); //enable STOP, disable START, Enable INIT TDC
    plx->write8(CS0,2,0x60); // enable TDC, enable WORK
    plx->write16(CS3,31*2,0xfc03); // enable START, enable STOP, Enable INIT TDC
}

void tdc::stop(){
#ifdef DEBUG
    std::cout << "STOP\n";
#endif
    plx->write16(CS3,31*2,0xf803); // enable STOP
    //plx->readMap(32,0,0,256*2+2);
}

data tdc::readDataOne(){
    struct data retval;
    uint16_t tmp;

    tmp = plx->read16(CS3,256*2);
    retval.channel_code = (tmp & 0xe000) >> 13;
    retval.time_code = (tmp & 0x1fff);

    return retval;
}

int tdc::memCheck(){
    char value = plx->read8(CS0,3);
#ifdef DEBUG
    std::cout << "CS0+3: " << std::hex << "0x" << (value&0xff) << std::dec << "\n";
#endif
    return (int)(value&0x03);
}
