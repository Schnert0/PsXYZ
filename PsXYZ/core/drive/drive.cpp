#include "drive.h"

Drive::Drive(){

}


Drive::~Drive(){

}


Uint32 Drive::read(Uint32 offset){
  switch(offset){
  default: printf("[CDD]\tunhandled cd drive read from offset %d\n", offset);
  }

  return 0;
}


void Drive::write(Uint32 offset, Uint32 data){
  switch(offset){
  default: printf("[CDD]\tunhandled cd drive write to offset %d with 0x%08x\n", offset, data);
  }
}
