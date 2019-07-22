#ifndef TDC_H
#define TDC_H

#include <iostream>
#include "plx9030c.h"

namespace TDC{

#define DEBUG

  /* memory check */
  enum{
    MEM_EMPTY,
    MEM_FULL,
    MEM_HALF
  };

  /* channel code */
  const unsigned int X1 = 3;
  const unsigned int Y1 = 1;
  const unsigned int Y2 = 5;
  const unsigned int X2 = 7;

 struct data{
   unsigned int channel_code;
   unsigned int time_code;
 };
 
 using namespace PLX9030;
 
 class tdc{
 private:
   plx9030 *plx;

 public:
   explicit tdc(std::string devname="/dev/plxdev0");
   ~tdc();
   void init(void);
   void start(void);
   void stop(void);
   int memCheck(void);

   data readDataOne();
   
 };

}

#endif
