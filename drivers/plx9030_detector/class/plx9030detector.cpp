#include "plx9030detector.h"

using namespace PLX9030Detector;

bool plx9030Detector::is_mem_end = false;
unsigned int plx9030Detector::mem_count = 0;

plx9030Detector::plx9030Detector(std::string device){
	fd = open(device.c_str(),O_RDWR);
	if(fd<0) status = ERROR_OPEN;
}

plx9030Detector::~plx9030Detector(){
	std::cout << "close file\n";
	close(fd);
}

void plx9030Detector::init(){
	is_mem_end = false;
	mem_count = 0;
	ioctl(fd,IOCTL_INIT_DETECTOR,0);
}

void plx9030Detector::start(){
	ioctl(fd,IOCTL_START_DETECTOR,0);
}

void plx9030Detector::stop(){
	ioctl(fd,IOCTL_STOP_DETECTOR,0);
}

raw_data plx9030Detector::readMem(){
	if(mem_count > MEMORY_SIZE) is_mem_end = true; 
	raw_data retval;
	uint16_t tmp;
	char buff[2];
	ioctl(fd,IOCTL_READ_DETECTOR,0);
	read(fd,buff,2);
	tmp = buff[0] | (buff[1] << 8);
  
	retval.raw = tmp;
	retval.code = (tmp & 0xe000) >> 13;
	retval.value = tmp & 0x1fff;
	if(retval.value > 4700) retval.value = -1; 

	mem_count += 2;
	
	return retval;
}

four_value plx9030Detector::read4Value(){
	  four_value retval;
	  retval.correct = true;

	  constexpr int MAX_ERRS = 100;
	  
	  raw_data data;
	  int value[4] = {-1,-1,-1,-1};

	  int count = 0;
	  int err = 0;
	  while(1){
		  data = readMem();
		  if(data.code==X1 || data.code==X2 || data.code==Y1 || data.code==Y2){
			  value[fromCode(data.code)] = data.value;
			  count ++;
			  if(count >= 4) break;
		  }else{
			  if(err > MAX_ERRS) break;
			  err ++;
		  }
		  
	  }
	  if(is_mem_end) retval.correct = false;
	  for(int i=0;i<4;i++) if(value[i] < 0) retval.correct = false;
	  
	  retval.y1 = value[fromCode(Y1)];
	  retval.x1 = value[fromCode(X1)];
	  retval.y2 = value[fromCode(Y2)];
	  retval.x2 = value[fromCode(X2)];
	  return retval;
}

unsigned char plx9030Detector::checkMem(){
	unsigned char byte = 0x00;
	ioctl(fd,IOCTL_TEST_DETECTOR,0);
	read(fd,&byte,1);
	return byte;
}
