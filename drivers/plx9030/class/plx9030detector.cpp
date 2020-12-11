#include "plx9030detector.h"

using namespace PLX9030Detector;

bool plx9030Detector::is_runing = false;
bool plx9030Detector::is_mem_end = false;
bool plx9030Detector::is_half_mem_end = false;
unsigned int plx9030Detector::mem_count = 0;

plx9030Detector::plx9030Detector(std::string device){
	plx = new PLX9030::plx9030(device);
	if(plx->getStatus()!=0) std::cout << "error open device!\n";
	
}

plx9030Detector::~plx9030Detector(){
	std::cout << "close device" << std::endl;
	delete plx;
}

void plx9030Detector::init(){
	std::cout << "init!" << std::endl;
	
	plx->write8(PLX9030::CS0,0,0);                         // RESET
	plx->write8(PLX9030::CS0,2,0);                         // RESET TDC & FIFO
	plx->write_hw16(PLX9030::CS3,31,0);                    // Disable start & stop
	plx->write8(PLX9030::CS0,2,0x40);                      // RESET TDC & FIFO
	plx->write_hw16(PLX9030::CS3,31,3);                    // Inable INIT

	usleep(300);
	
	plx->write_hw16(PLX9030::CS3,1,0x0007);
	plx->write_hw16(PLX9030::CS3,0,0xfc81);

	plx->write_hw16(PLX9030::CS3,3,0);
	plx->write_hw16(PLX9030::CS3,2,0);

	plx->write_hw16(PLX9030::CS3,5,0);
	plx->write_hw16(PLX9030::CS3,4,2);

	plx->write_hw16(PLX9030::CS3,7,0);
	plx->write_hw16(PLX9030::CS3,6,0);

	plx->write_hw16(PLX9030::CS3,9,0x600);
	plx->write_hw16(PLX9030::CS3,8,2);

	plx->write_hw16(PLX9030::CS3,11,0x0e0);
	plx->write_hw16(PLX9030::CS3,10,0);

	plx->write_hw16(PLX9030::CS3,13,0);
	plx->write_hw16(PLX9030::CS3,12,0);

	plx->write_hw16(PLX9030::CS3,15,0x014);
	plx->write_hw16(PLX9030::CS3,14,0x1fb4);

	plx->write_hw16(PLX9030::CS3,23,0x7ff);
	plx->write_hw16(PLX9030::CS3,22,0);

	plx->write_hw16(PLX9030::CS3,25,0x200);
	plx->write_hw16(PLX9030::CS3,24,0);

	plx->write_hw16(PLX9030::CS3,29,0);
	plx->write_hw16(PLX9030::CS3,28,0);

	plx->write_hw16(PLX9030::CS3,9,0x640);
	plx->write_hw16(PLX9030::CS3,8,0);
	
	usleep(100000);

	// Check
	unsigned short int tmp;

	/*
	for(int i=0;i<32;i++){
		tmp = (unsigned short int)(plx->read_hw16(PLX9030::CS3,i)&0xffff);
		std::cout <<  "(16 bit) CS3+" << i << std::hex << ": 0x" << tmp << std::dec << std::endl;
										       
	}
	*/

	std::cout << "Check mem: 0x" << std::hex << (int)checkMem() << std::dec <<std::endl;
}

void plx9030Detector::start(){
	std::cout << "start!\n";
	
	plx->write8(PLX9030::CS0,0,0x80);
	plx->write8(PLX9030::CS0,2,0x40);
	plx->write_hw16(PLX9030::CS3,31,0xf001);
	plx->write8(PLX9030::CS0,2,0x60);
	plx->write_hw16(PLX9030::CS3,31,0xfc03);
	
}

void plx9030Detector::stop(){
	plx->write_hw16(PLX9030::CS3, 31, 0xf803);
	std::cout << "stop!\n";
}

raw_data plx9030Detector::readMem(){
	raw_data retval;
	uint16_t tmp;

	tmp = (unsigned short int)(plx->read_hw16(PLX9030::CS3,256)&0xffff);
	retval.raw = tmp;
	retval.code = (tmp & 0xe000) >> 13;
	retval.value = tmp & 0x1fff;
	
	return retval;
}

std::vector<raw_data> plx9030Detector::getAllMemory(void){
	std::vector<raw_data> retval;

	raw_data mem_val;
	int count = 0;
	int watchdog = 0;
	while(1){
		if(count>1) if(checkMem() & 0x04) break;
		mem_val = readMem();
		if(mem_val.raw!=0) retval.push_back(mem_val);
		count ++;
		// WatchDog:
		if(count>5) if(retval.at(retval.size()-1).code == retval.at(retval.size()-2).code) watchdog ++;
		if(watchdog > 10) break;
	}
	
	return retval;
}

std::vector<four_value> plx9030Detector::convertToFourValue(std::vector<raw_data> raw_values){
	std::vector<four_value> retval;
	four_value four;
	int value[4] = {-1,-1,-1,-1};
	
	
	for(int i=3;i<raw_values.size();i+=4){
		four.correct = true;
		value[fromCode(raw_values.at(i-3).code)] = raw_values.at(i-3).value;
		value[fromCode(raw_values.at(i-2).code)] = raw_values.at(i-2).value;
		value[fromCode(raw_values.at(i-1).code)] = raw_values.at(i-1).value;
		value[fromCode(raw_values.at(i).code)] = raw_values.at(i).value;
		
		four.y1 = value[fromCode(Y1)];
		four.x1 = value[fromCode(X1)];
		four.y2 = value[fromCode(Y2)];
		four.x2 = value[fromCode(X2)];

		for(int j=0;j<4;j++) if(value[j]<0) four.correct = false;
				
		retval.push_back(four);
	}
	
	return retval;
}

unsigned char plx9030Detector::checkMem(){
	unsigned char byte = 0x00;
	byte = plx->read8(PLX9030::CS0,3);
	byte &= 0x0f;
	return byte;
}
