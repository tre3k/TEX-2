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
  /* initialisation */
  /* D7 - Disable INTR from falg FIFO */
  plx->write8(CS0,0,0x00);  // CS0+0: D7 = 0, D6..D0 = xx;
  /* D5 - reset TDC, D6 - reset FIFO */
  plx->write8(CS0,2,0x00);  // CS0+2: D6,D5 = 0, D7,D4..D0 = xx;
  /* disble stop, disable start */
  plx->write16(CS3,31*2,0x0000);

  /* Enable TDC */
  plx->write8(CS0,2,0x40);           // CS0+2: D6 = 1
  plx->write16(CS3,31*2,0x0003);     // D0=1,D1=1

  /* init TDC*/
  plx->write16(CS3,1*2,0x0007);     
  plx->write16(CS3,0*2,0xfc81);
  
  plx->write16(CS3,3*2,0x0000);
  plx->write16(CS3,2*2,0x0000);

  plx->write16(CS3,5*2,0x0000);
  plx->write16(CS3,4*2,0x0002);

  plx->write16(CS3,7*2,0x0000);
  plx->write16(CS3,6*2,0x0000);

  plx->write16(CS3,9*2,0x0600);
  plx->write16(CS3,8*2,0x0000);

  plx->write16(CS3,11*2,0x00e0);
  plx->write16(CS3,10*2,0x0000);

  plx->write16(CS3,13*2,0x0000);
  plx->write16(CS3,12*2,0x0000);

  plx->write16(CS3,15*2,0x0014);
  plx->write16(CS3,14*2,0x1fb4);

  plx->write16(CS3,23*2,0x07ff);  //0x000007ff
  plx->write16(CS3,22*2,0x0000);
  
  plx->write16(CS3,25*2,0x0200);
  plx->write16(CS3,24*2,0x0000);

  plx->write16(CS3,29*2,0x0000);
  plx->write16(CS3,28*2,0x0000);

  plx->write16(CS3,9*2,0x0640); // Master reset
  plx->write16(CS3,8*2,0x0000);

  return;
}

void tdc::start(){
  plx->write8(CS0,0,0x80); // D7 = 1, enable INTR
  plx->write8(CS0,2,0x40); // enable TDC

  plx->write16(CS3,31*2,0xf001); //enable STOP, disable START, Enable INIT TDC
  plx->write8(CS0,2,0x60); // enable TDC, enable WORK
  plx->write16(CS3,31*2,0xfc03); // enable START, enable STOP, Enable INIT TDC
}

void tdc::stop(){
  plx->write16(CS3,31*2,0xf803); // enable STOP
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
  return (int)(value&0x03);
}
