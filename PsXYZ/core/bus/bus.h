#ifndef memory_h
#define memory_h

#include <SDL.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "../gpu/gpu.h"
#include "../dma/dma.h"

class DMA;

Uint32 read(Uint32 offset);
void write(Uint32 offset, Uint32 data);


class Bus{
private:
    GPU* gpu;
    DMA* dma;

    Uint32 mask[8];

    union{
        Uint8  b[RAM_SIZE    ];
        Uint16 h[RAM_SIZE / 2];
        Uint32 w[RAM_SIZE / 4];
    }ram;

    union{
        Uint8  b[BIOS_SIZE    ];
        Uint16 h[BIOS_SIZE / 2];
        Uint32 w[BIOS_SIZE / 4];
    }bios;

public:
     Bus();
    ~Bus();
    void getComponents(GPU* gpuPtr, DMA* dmaPtr);

    void reset();

    void loadBIOS(const char* path);

    Uint32 load(Uint32 address, Uint8 width);
    void  store(Uint32 address, Uint32 data, Uint8 width);

};

#endif // memory_h
