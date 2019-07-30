#include <iostream>
#include "plx9030detector.h"

using namespace PLX9030Detector;

int main(int argc,char **argv){
  plx9030Detector *pd = new plx9030Detector("/dev/plxdetector0");
  pd->init();
  pd->start();
  sleep(10);
  pd->stop();
  for(int i=0;i<10;i++){
    std::cout << std::hex << "0x" << pd->readMem() << "\n";
  }

  std::cout << "check: " << std::dec << (int)(pd->checkMem()&0xff) << "\n";
  
  delete pd;
  return 0;
}
