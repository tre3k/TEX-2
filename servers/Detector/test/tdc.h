#ifndef TDC_H
#define TDC_H

#include <iostream>
#include "plx9030c.h"

enum{
  TDC_MEM_EMPTY,
  TDC_MEM_FULL,
  TDC_MEM_HALF
};

using namespace PLX9030;

class TDC{
 private:
  plx9030 *plx;

 public:
  explicit TDC(std::string devname="/dev/plxdev0");
  ~TDC();
  void init(void);
  void start(void);
  void stop(void);
  int memCheck(void);
};



#endif
