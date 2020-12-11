#include <iostream>
#include "plx9030c.h"
#include "plx9030detector.h"

int main(int argc,char **argv){
	PLX9030Detector::plx9030Detector *det = new PLX9030Detector::plx9030Detector("/dev/plxdev0");
	det->init();
	det->start();
	sleep(10);
	det->stop();
	//sleep(1);

	std::cout << std::hex << "Check: 0x" << (int) det->checkMem() << std::dec << std::endl;

	std::vector<PLX9030Detector::raw_data> values;
	std::vector<PLX9030Detector::four_value> values4;

	values = det->getAllMemory();
	values4 = det->convertToFourValue(values);

	for(auto val: values4){
		std::cout << " x1 = " << val.x1
			  << ", x2 = " << val.x2
			  << ", y1 = " << val.y1
			  << ", y2 = " << val.y2;
		if(!val.correct) std::cout << " NOT CORRECT!";
		std::cout << std::endl;
	}
	
	
	/*
	for(auto val : values){
		std::cout << std::dec << val.code << " " << val.value << std::hex << "  <- 0x" << val.raw  << std::dec << std::endl;
	}
	std::cout << "size : " << values.size() << std::endl;
	*/

	/*
	for(int i=0;i<999;i++){
		mem_val = det->readMem();
		std::cout << std::dec << i <<  "\r\t" <<  mem_val.code << " " << mem_val.value << std::hex << "  <- 0x" << mem_val.raw << std::endl;
		if(i>1) std::cout << std::hex << "0x" << (int) det->checkMem() << std::dec << std::endl;

	}
	*/
	
	delete det;
	std::cout << "exit." << std::endl;
}
