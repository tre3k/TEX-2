#include "plx9030detector.h"

using namespace PLX9030Detector;

plx9030Detector::plx9030Detector(std::string device){
  fd = open(device.c_str(),O_RDWR);
  if(fd<0) status = ERROR_OPEN;
}

plx9030Detector::~plx9030Detector(){
  std::cout << "close file\n";
  close(fd);
}

void plx9030Detector::init(){
  ioctl(fd,IOCTL_INIT_DETECTOR,0);
}

void plx9030Detector::start(){
  ioctl(fd,IOCTL_START_DETECTOR,0);
}

void plx9030Detector::stop(){
  ioctl(fd,IOCTL_STOP_DETECTOR,0);
}

raw_data plx9030Detector::readMem(){
  raw_data retval;
  uint16_t tmp;
  char buff[2];
  ioctl(fd,IOCTL_READ_DETECTOR,0);
  read(fd,buff,2);
  tmp = buff[0] | (buff[1] << 8);

  retval.raw = tmp;
  retval.code = (tmp & 0xe000) >> 13;
  retval.value = tmp & 0x1fff;
  
  return retval;
}

four_value plx9030Detector::read4Value(){
  four_value retval;
  bool success = false;
  int codes[] = {Y1,X1,Y2,X2};
  int value[4];
  
  raw_data data;
  int j = 0;
  const int skeep_count = 100;
  
  do{
    if(j > skeep_count){
      retval.correct = false;
      return retval;
    }
    j++;
    
    data = readMem();
    if(data.code!=Y1) continue;
    value[0] = data.value;
    success = true;
    
    for(int i=1;i<4;i++){
      data = readMem();
      if(data.code != codes[i]){
	success = false;
	continue;
      }
      value[i] = data.value;
    }
    
  } while(!success);

  retval.correct = success;

  retval.y1 = value[0];
  retval.x1 = value[1];
  retval.y2 = value[2];
  retval.x2 = value[3];
  
  return retval;
}

unsigned char plx9030Detector::checkMem(){
  unsigned char byte = 0x00;
  ioctl(fd,IOCTL_TEST_DETECTOR,0);
  read(fd,&byte,1);
  return byte;
}
