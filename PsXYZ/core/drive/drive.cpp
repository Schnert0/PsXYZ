#include "drive.h"

Drive::Drive(){

}


Drive::~Drive(){

}


void Drive::getComponents(CPU* cpuPtr){
  cpu = cpuPtr;
}


void Drive::reset(){
  stat.raw = 0b11000;

  cmdReady   = 0;
  cmdReg     = 0;
  intEnable  = 0;
  requestReg = 0;
  intFlagReg = 0;

  pfLen = 0;
  rfLen = 0;
}


Uint32 Drive::read(Uint32 offset){
  if(offset != 0)
    printf("[CDD]\tread from offset %d.%d\n", offset, stat.index);
  else
    printf("[CDD]\tread from offset 0 and got 0x%08x\n", stat.raw);

  switch(offset){
  case 0:
    return stat.raw | 0b1000;

  default: printf("[CDD]\tunhandled cd drive read from offset %d\n", offset);
  }

  return 0;
}


void Drive::write(Uint32 offset, Uint32 data){
  if(offset != 0)
    printf("[CDD]\twrite to offset %d.%d with 0x%08x\n", offset, stat.index, data);
  else
    printf("[CDD]\twrite to offset 0   with 0x%08x\n", data);

    Uint8 currInt = intFlagReg & 0x1f;
    Uint8 ackInt = data & 0x1f;

  switch(offset){
  case 0:
    stat.index = (data & 3);
    break;

  case 1:
    switch(stat.index){
    case 0:
      cmdReg = data;
      cmdReady = true;
      break;

    default: printf("[CDD]\tunhandled cd drive write to offset 1.%d\n", stat.index);
    }
    break;

  case 2:
    switch(stat.index){
    case 0:
      if(pfLen < 16){
        paramFifo[pfLen] = data;
        pfLen++;
        stat.PRMEMPT = false;
        if(pfLen >= 16)
          stat.PRMWRDY = true;
      }
      break;

    case 1:
      intEnable = data;
      break;

    default: printf("[CDD]\tunhandled cd drive write to offset 2.%d\n", stat.index);
    }
    break;

  case 3:
    switch(stat.index){
    case 1:
      intFlagReg &= ~ackInt;
      if(intFlagReg & 0x40){
        pfLen = 0;
        stat.PRMEMPT = true;
        stat.PRMWRDY = true;

      }
      if((ackInt & currInt) == currInt){
        //printf("recieved int ack\n");
        setInterrupt(0);
      }
      break;
    default: printf("[CDD]\tunhandled cd drive write to offset 3.%d\n", stat.index);
    }
    break;

  default: printf("[CDD]\tunhandled cd drive write to offset %d with 0x%08x\n", offset, data);
  }

  if(cmdReady){
    cmdReady = false;

    Uint8 biosRevision[] = { 0x95, 0x05, 0x16, 0xc1 };

    switch(cmdReg){
    case 0x19:
      switch(paramFifo[0]){
      case 0x20:
        printf("[CDD]\tget BIOS date/revision\n");
        pushResponse(biosRevision, sizeof(biosRevision));
        break;
      default: printf("[CDD]\terror - unimplemented cd drive opcode 0x19.0x%02x\n", paramFifo[0]);
      }
      break;
    default: printf("[CDD]\terror - unimplemented cd drive opcode 0x%02x\n", cmdReg);
    }
    setInterrupt(3);
  }
}


void Drive::updateStat(){
  stat.ADPBUSY = false;
  stat.PRMEMPT = (pfLen == 0);
  stat.PRMWRDY = (pfLen >= 16);
  stat.RSLRDY  = (rfLen == 0);
  stat.DRQSTS  = false;
  stat.BUSYSTS = false;
}


void Drive::pushResponse(Uint8* src, Uint8 size){
  for(int i = 0; i < size; i++){
    responseFifo[i] = src[i];
  }
  rfLen = 0;
}


Uint8 Drive::popResponse(){
  if(rfLen < 16){
    Uint8 data = responseFifo[rfLen];
    rfLen++;
    return data;
  }
  return 0xff;
}


void Drive::setInterrupt(Uint8 interrupt){
  if(!(intEnable & 0x10))interrupt |= 0x10;
  intFlagReg &= ~0x1f;
  intFlagReg |= interrupt & 0x1f;
  if(interrupt & intEnable){
    //printf("sending int to CPU\n");
    cpu->interruptRequest(IRQ_CDROM);
  }
}
