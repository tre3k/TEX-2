#include <iostream>

#include "plx9030c.h"

int main(int argc, char **argv){
  char byte;

  PLX9030::plx9030 *plx = new PLX9030::plx9030("/dev/plxdev0");

  plx->write8(0x00, PLX9030::CS0,0);
  
  for(int i=0;i<32;i++){
    byte = plx->read8(PLX9030::CS0, i);
    std::cout << std::dec << "CS0+" << i
	      << " : 0x" << std::hex << (byte&0xff) << "\t|";

    byte = plx->read8(PLX9030::CS1, i);
    std::cout << std::dec << "CS1+" << i
	      << " : 0x" << std::hex << (byte&0xff) << "\t|";
  
    byte = plx->read8(PLX9030::CS2, i);
    std::cout << std::dec << "CS2+" << i
	      << " : 0x" << std::hex << (byte&0xff) << "\t|";

    byte = plx->read8(PLX9030::CS3, i);
    std::cout << std::dec << "CS3+" << i
	      << " : 0x" << std::hex << (byte&0xff) << "\n";
  
  
  }

  delete plx;
  return 0;
}
