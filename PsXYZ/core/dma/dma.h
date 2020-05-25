#ifndef dma_h
#define dma_h

#include <SDL.h>
#include <stdio.h>
#include <string.h>

#include "../gpu/gpu.h"
#include "../bus/bus.h"

typedef struct{
    Uint32 baseAddr;
    union{
        Uint32 block;
        struct{
            Uint16 blockSize  : 16;
            Uint16 blockCount : 16;
        };
    };

    union{
        Uint32 ctrl;
        struct {
            Uint32 dir     :1;
            Uint32 step    :1;
            Uint32         :6;
            Uint32 chop    :1;
            Uint32 sync    :2;
            Uint32         :5;
            Uint32 chopDMA :3;
            Uint32         :1;
            Uint32 chopCPU :3;
            Uint32         :1;
            Uint32 enable  :1;
            Uint32         :3;
            Uint32 trigger :1;
            Uint32         :3;
        };
    };
}s_chan;

class c_mem;

class DMA{
private:
    GPU* gpu;
    class Bus* bus;

    s_chan channel[8];
    Uint32 control;

    struct{
        bool  irqEnable;
        Uint8 chanIrqEnable;
        Uint8 chanIrqFlags;
        bool  forceIrq;
        Uint8 dummy;
    };

    Uint32 getInterrupt();
    void setInterrupt(Uint32 data);

    Uint32 transferSize(Uint8 chan);
    bool dmaActive(Uint8 chan);
    void  doDMA(Uint8 chan);
    void doBlock(Uint8 chan);
    void doLinkedList(Uint8 chan);

public:
     DMA();
    ~DMA();
    void getComponents(GPU* gpuPtr, Bus* busPtr);

    void reset();

    Uint32 read(Uint32 offset);
    void  write(Uint32 offset, Uint32 data);
};

#endif // dma_h
