#ifndef PLX9030DETECTOR_H
#define PLX9030DETECTOR_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stropts.h>
#include <vector>

#include "plx9030c.h"


namespace PLX9030Detector{

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
	std::vector<raw_data> getAllMemory(void);
	std::vector<four_value> convertToFourValue(std::vector<raw_data>);
	unsigned char checkMem(void);

public:
	int status;
	static bool is_runing;
	static bool is_mem_end;
	static bool is_half_mem_end;
	static unsigned int mem_count;
	
private:
	PLX9030::plx9030 *plx = nullptr;
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
