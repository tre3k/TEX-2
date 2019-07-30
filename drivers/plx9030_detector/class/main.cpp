#include <iostream>
#include "plx9030detector.h"

using namespace PLX9030Detector;

int main(int argc,char **argv){
  plx9030Detector *pd = new plx9030Detector("/dev/plxdetector0");
  raw_data data;

  if(argc < 2){
    std::cout << argv[0] << " <time in sec>\n";
    return 0;
  }
  
  std::cout << "start " << atoi(argv[1]) << " sec\n";
  
  pd->init();
  pd->start();
  sleep(atoi(argv[1]));
  pd->stop();

  //low format
  /*
  for(int i=0;i<100;i++){
    data = pd->readMem();
    
    std::cout << std::dec << data.code
	      << std::dec << " :\t" << data.value
	      << std::hex << "\t0x" << data.raw <<  "\n";
    
  }
  std::cout << "check: " << std::dec << (int)(pd->checkMem()&0xff) << "\n";

  */

  
  four_value fdata;
  
  std::cout << "x1\tx2\ty1\ty2\n";
  for(int i=0;i<100;i++){
    fdata = pd->read4Value();
    std::cout << fdata.x1 << "\t"
	      << fdata.x2 << "\t"
	      << fdata.y1 << "\t"
	      << fdata.y2 << "\n";
  }

  std::cout << "memory is ";
    switch(pd->checkMem()){
  case MEMORY_EMPTY:
    std::cout << "empty.\n";
    break;
  case MEMORY_HALF:
    std::cout << "half loaded.\n";
    break;
  case MEMORY_FULL:
    std::cout << "full.\n";
    break;
  }

  

      
  delete pd;
  return 0;
}
