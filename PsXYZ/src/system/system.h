#ifndef system_h
#define system_h

#include "../gpu/gpu.h"
#include "../dma/dma.h"
#include "../memory/memory.h"
#include "../cpu/cpu.h"

class c_system{
private:
    c_gpu* gpu;
    c_dma* dma;
    c_mem* mem;
    c_cpu* cpu;

public:
     c_system();
    ~c_system();

    void loadBIOS(const char* path);

    void reset();
    void run();

};

#endif // system_h
