#ifndef PLX9030DETECTOR_H
#define PLX9030DETECTOR_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stropts.h>

namespace PLX9030Detector{

#define IOCTL_INIT_DETECTOR 0xfacecafe             // for initialization detector
#define IOCTL_START_DETECTOR IOCTL_INIT_DETECTOR+1 // for start measure
#define IOCTL_STOP_DETECTOR IOCTL_INIT_DETECTOR+2  // for stop measure
#define IOCTL_READ_DETECTOR IOCTL_INIT_DETECTOR+3  // for read data from FIFO
#define IOCTL_TEST_DETECTOR IOCTL_INIT_DETECTOR+4  // for test memory half/full/empty

enum{
  MEMORY_EMPTY,
  MEMORY_FULL,
  MEMORY_HALF
};

 
class plx9030Detector{
 public:
  explicit plx9030Detector(std::string device);
  ~plx9030Detector();
  void init(void);
  void start(void);
  void stop(void);
  uint16_t readMem(void);
  unsigned char checkMem(void);
  
 private:
  int fd;
};


}


#endif // PLX9030DETECTOR_H
