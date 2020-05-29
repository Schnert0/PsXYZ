#ifndef drive_h
#define drive_h

#include <SDL.h>
#include <stdio.h>
#include <string.h>

#include "../cpu/cpu.h"

class CPU;

class Drive{
private:
  CPU* cpu;

  bool   cmdReady;
  Uint32 cmdReg;
  Uint32 intEnable;
  Uint32 requestReg;
  Uint32 intFlagReg;

  Uint32 paramFifo[16];
  Uint32 responseFifo[16];
  Uint8  pfLen;
  Uint8  rfLen;

  union{
    Uint8 raw;
    struct{
      Uint8 index   : 2;
      bool  ADPBUSY : 1;
      bool  PRMEMPT : 1;
      bool  PRMWRDY : 1;
      bool  RSLRDY  : 1;
      bool  DRQSTS  : 1;
      bool  BUSYSTS : 1;
    };
  }stat;

  void updateStat();

  void pushResponse(Uint8* src, Uint8 size);
  Uint8 popResponse();

  void setInterrupt(Uint8 irq);

public:
   Drive();
  ~Drive();
  void getComponents(CPU* cpuPtr);

  void reset();

  Uint32 read(Uint32 offset);
  void  write(Uint32 offset, Uint32 data);
};

#endif // drive_h
