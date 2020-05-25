#ifndef system_h
#define system_h

#include "../../renderer/renderer.h"

#include "../gpu/gpu.h"
#include "../dma/dma.h"
#include "../bus/bus.h"
#include "../cpu/cpu.h"
#include "../drive/drive.h"

class System{
private:
  Renderer* renderer;

  GPU*   gpu;
  DMA*   dma;
  Bus*   bus;
  CPU*   cpu;
  Drive* drive;

public:
   System(Renderer* rendererPtr);
  ~System();

  void loadBIOS(const char* path);

  void reset();
  void run();

};

#endif // system_h
