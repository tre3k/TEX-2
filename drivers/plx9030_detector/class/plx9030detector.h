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

	/* for status */
	enum{
		OK,
		ERROR_OPEN
	};

#define MEMORY_SIZE 524288 //bytes
 
#define MEMORY_EMPTY 0
#define MEMORY_HALF 2
#define MEMORY_FULL 3  

#define Y2 3
#define Y1 7
#define X2 1
#define X1 5

  
struct raw_data{
	int code;
	int value;
	uint16_t raw;
};

struct four_value{
	int x1,x2,y1,y2;
	bool correct;
};

class plx9030Detector{
public:
	explicit plx9030Detector(std::string device);
	~plx9030Detector();
	void init(void);
	void start(void);
	void stop(void);
	raw_data readMem(void);
	void readAllMem(four_value *data);
	four_value read4Value(void);
	unsigned char checkMem(void);
public:
	int status;
	static bool is_runing;
	static bool is_mem_end;
	static bool is_half_mem_end;
	static unsigned int mem_count;
	
private:
	int fd;
	int fromCode(int code){
		switch(code){
		case X1:
			return 0;
		case X2:
			return 1;
		case Y1:
			return 2;
		case Y2:
			return 3;
		}
	}
};

}

#endif // PLX9030DETECTOR_H
