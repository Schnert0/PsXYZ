#include "dma.h"

DMA::DMA(){
}


DMA::~DMA(){
}


void DMA::getComponents(GPU* gpuPtr, Bus* busPtr){
    gpu = gpuPtr;
    bus = busPtr;
}


void DMA::reset(){
    memset(channel, 0, sizeof(channel));
    control = 0x07654321;
}


Uint32 DMA::read(Uint32 offset){
    //printf("[DMA]\tread from 0x%02x\n", offset);

    Uint8 chan = (offset & 0x70) >> 4;
    Uint8 reg  = (offset & 0x0f);

    switch(chan){
    case 0 ... 6:
        switch(reg){
        case 0x00: return channel[chan].baseAddr;
        case 0x04: return channel[chan].block;
        case 0x08: return channel[chan].ctrl;
        default: printf("[DMA]\tunhandled channel %d register read 0x%02x\n", chan, reg); getchar();
        }
        break;
    case 7:
        switch(reg){
        case 0x00: return control;
        case 0x04: return getInterrupt();
        default: printf("[DMA]\tunhandled channel %d register read 0x%02x\n", chan, reg); getchar();
        }
        break;
    default: printf("[DMA]\tunhandled DMA channel %d read\n", chan); getchar();
    }
    return 0;
}


void DMA::write(Uint32 offset, Uint32 data){
    //printf("[DMA]\twrite 0x%08x to 0x%02x\n", data, offset);

    Uint8 chan = (offset & 0x70) >> 4;
    Uint8 reg  = (offset & 0x0f);

    switch(chan){
    case 0 ... 6:
        switch(reg){
        case 0x00: channel[chan].baseAddr = data & 0xffffff; break;
        case 0x04: channel[chan].block = data; break;
        case 0x08: channel[chan].ctrl = data; break;
        default: printf("[DMA]\tunhandled channel %d register write 0x%02x with 0x%08x\n", chan, reg, data); getchar();
        }
        break;
    case 7:
        switch(reg){
        case 0x00: control = data; break;
        case 0x04: setInterrupt(data); break;
        default: printf("[DMA]\tunhandled channel 7 register write 0x%02x with 0x%08x\n", reg, data); getchar();
        }
        break;
    default: printf("[DMA]\tunhandled DMA channel %d write with 0x%08x\n", chan, data); getchar();
    }

    if(dmaActive(chan))
        doDMA(chan);
}


Uint32 DMA::getInterrupt(){
    Uint32 reg = 0;

    reg |=  dummy;
    reg |=  forceIrq << 15;
    reg |=  chanIrqEnable << 16;
    reg |=  irqEnable << 23;
    reg |=  chanIrqFlags << 24;
    reg |= (forceIrq || (irqEnable && (chanIrqFlags & chanIrqEnable))) << 31;

    return reg;
}


void DMA::setInterrupt(Uint32 data){
    dummy = data & 0x3f;
    forceIrq = (data >> 15) & 1;
    chanIrqEnable = (data >> 16) & 0x7f;
    irqEnable = (data >> 23) & 1;
    chanIrqFlags &= ~((data >> 24) & 0x3f);
}

Uint32 DMA::transferSize(Uint8 chan){
    switch(channel[chan].sync){
    case 0: return channel[chan].blockSize; break;
    case 1: return channel[chan].blockCount * channel[chan].blockSize; break;
    case 2: return 0; break;
    default: printf("[DMA]\terror - undefined DMA sync mode\n"); getchar();
    }
    return 0;
}

bool DMA::dmaActive(Uint8 chan){
     bool trigger;
    switch(channel[chan].sync){
    case 0:  trigger = channel[chan].trigger; break;
    default: trigger = true;
    }
    return channel[chan].enable && trigger;
}

void DMA::doDMA(Uint8 chan){
    switch(channel[chan].sync){
    case 0:
    case 1: doBlock(chan); break;
    case 2: doLinkedList(chan); break;
    default: printf("[DMA]\terror - undefined DMA channel sync mode %d\n", channel[chan].sync); getchar();
    }

    channel[chan].enable  = false;
    channel[chan].trigger = false;
}

void DMA::doBlock(Uint8 chan){
    //printf("[DMA]\tblock copy started\n");

    Sint32 step = 0;
    switch(channel[chan].step){
    case 0: step =  4; break;
    case 1: step = -4; break;
    }

    Uint32 addr = channel[chan].baseAddr & 0x1ffffc;
    Uint32 dataSize = transferSize(chan);

    //printf("Base Address:\t0x%06x\nData Size:\t%d\n", addr, dataSize);

    Uint32 src;
    while(dataSize > 0){
        switch(channel[chan].dir){
        case 0:
            switch(chan){
            case 6:
                switch(dataSize){
                case 1:  src = 0xffffff; break;
                default: src = (addr - 4) & 0x1fffff;
                }
                break;
            default: printf("[DMA]\terror unhandled DMA channel %d\n", chan); getchar();
            }
            //printf("0x%08x <= 0x%08x\n", addr, src);
            bus->store(addr, src, WIDTH_WORD);
            break;
        case 1:
            src = bus->load(addr, WIDTH_WORD);
            switch(chan){
            case 2: gpu->gp0Write(src); break;
            default: printf("[DMA]\terror - undefined DMA channel %d\n", chan); getchar();
            }
            break;
        }
        addr += step;
        dataSize--;
    }
    //printf("[DMA]\tblock copy complete\n");

}

void DMA::doLinkedList(Uint8 chan){
    //printf("[DMA]\tlinked list started\n");

    Uint32 addr = channel[chan].baseAddr & 0x1ffffc;
    if(channel[chan].dir == 0){
        printf("[DMA]\terror - undefined toRam\n");
        getchar();
    }
    bool running = true;
    while(running){
        Uint32 header = bus->load(addr, WIDTH_WORD);
        Uint8 packetSize = header >> 24;
        //printf("[DMA]\theader:\t0x%08x\nsize:\t0x%02x\nnextPacket:\t0x%06x\n", header, packetSize, header & 0xffffff);
        while(packetSize > 0){
            addr = (addr+4) & 0x1ffffc;
            Uint32 data = bus->load(addr, WIDTH_WORD);
            gpu->gp0Write(data);
            packetSize--;
        }

        addr = header & 0xffffff;
        if(addr & 0x800000)
            running = false;
        //else printf("[DMA]\tnext packet...\n\n");
    }

    //printf("[DMA]\tlinked list done\n");
}
