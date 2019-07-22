#include <iostream>

#include "tdc.h"

int main(int argc, char **argv){
  char byte;
  TDC::data TDCdata;
  
  TDC::tdc *tdc = new TDC::tdc();

  //tdc->start();
  //sleep(10);
  //tdc->stop();

  std::cout << "\n";
  switch(tdc->memCheck()){
  case TDC::MEM_EMPTY:
    std::cout << "memory empty\n";
    break;

  case TDC::MEM_HALF:
    std::cout << "memory half\n";
    break;

  case TDC::MEM_FULL:
    std::cout << "memory full\n";
    break;
  }

  
  for(int i=0;i<16;i++){
    TDCdata = tdc->readDataOne();
    std::cout << std::dec << TDCdata.channel_code << " : "
	      << TDCdata.time_code << "\n";
  }

  
  delete tdc;
  
  return 0;
}
