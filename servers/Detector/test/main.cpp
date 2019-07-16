#include <iostream>

#include "tdc.h"

int main(int argc, char **argv){
  char byte;

  TDC *tdc = new TDC();

  std::cout << "\n";
  switch(tdc->memCheck()){
  case TDC_MEM_EMPTY:
    std::cout << "memory empty\n";
    break;

  case TDC_MEM_HALF:
    std::cout << "memory half\n";
    break;

  case TDC_MEM_FULL:
    std::cout << "memory full\n";
    break;
  }
  
  delete tdc;
  
  return 0;
}
