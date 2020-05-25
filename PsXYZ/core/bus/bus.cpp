#include "bus.h"

Bus::Bus(){
    const Uint32 tmp[] = {
        0x7fffffff,
        0x7fffffff,
        0x7fffffff,
        0x7fffffff,
        0x1fffffff,
        0x1fffffff,
        0xffffffff,
        0xffffffff
    };
    memcpy(mask, tmp, sizeof(mask));
}

Bus::~Bus(){
}

void Bus::getComponents(CPU* cpuPtr, GPU* gpuPtr, DMA* dmaPtr, Drive* drivePtr){
    cpu   = cpuPtr;
    gpu   = gpuPtr;
    dma   = dmaPtr;
    drive = drivePtr;
}

void Bus::reset(){
    memset(ram.b, 0, RAM_SIZE);
}

void Bus::loadBIOS(const char* path){
    FILE* file = fopen(path, "rb");
    if(!file){
        printf("[BUS]\tERROR: unable to load \"%s\"\n", path);
    }
    fseek(file, 0, SEEK_END);
    Uint32 fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("[BUS]\tBIOS is %d bytes\n", fileSize);

    if(fileSize == 524288){
        fread(bios.b, BIOS_SIZE, sizeof(Uint8), file);
    }else{
        printf("[BUS]\tERROR: incorrect file size for BIOS\n");
    }
    fclose(file);
    printf("[BUS]\tBIOS loaded successfully\n");
}

Uint32 Bus::load(Uint32 address, Uint8 width){
    Uint32 addr = address & mask[address >> 29];
    if(IN_RANGE(RAM_START, RAM_SIZE)){
        switch(width){
        case WIDTH_BYTE: return ram.b[(addr - RAM_START)     ];
        case WIDTH_HALF: return ram.h[(addr - RAM_START) >> 1];
        case WIDTH_WORD: return ram.w[(addr - RAM_START) >> 2];
        default: printf("[BUS]\tERROR: undefined width %d\n", width);
        }
    }
    else if(IN_RANGE(EXP1_START, EXP1_SIZE)){
        switch(width){
        case WIDTH_BYTE: return 0xff;
        case WIDTH_HALF: return 0xffff;
        case WIDTH_WORD: return 0xffffffff;
        default: printf("[BUS]\tERROR: undefined width %d\n", width);
        }
    }
    else if(IN_RANGE(IO_START, IO_SIZE)){
      switch(addr){
        case 0x1f801000 ... 0x1f801020: // memory ctrl registers
          break;

        case 0x1f801060: // RAM_SIZE register
          break;

        case 0x1f801070 ... 0x1f801074: // I_STAT and I_MASK
          cpu->readInterrupt(addr - 0x1f801070);
          break;

        case 0x1f801080 ... 0x1f8010ff: // DMA registers
          return dma->read(addr - 0x1f801080);

        case 0x1f801100 ... 0x1f80112f: // timers
          break;

        case 0x1f801800 ... 0x1f801803: // cd drive registers
          drive->read(addr - 0x1f801800);
          break;

        case 0x1f801810: // GP0 register
          break;

        case 0x1f801814: // GP1 register
          //return gpu->gp1Read();
          return 0x1c000000;
          break;

        case 0x1f801c00 ... 0x1f801e80: // various SPU registers
          break;

        case 0x1f802000 ... 0x1f802067: // Expansion region 2 registers;
          break;

        default:
          printf("[BUS]\tERROR: undefined IO load at 0x%08x\n", address);
      }
      return 0;
    }
    else if(IN_RANGE(BIOS_START, BIOS_SIZE)){
        switch(width){
        case WIDTH_BYTE: return bios.b[(addr - BIOS_START)     ];
        case WIDTH_HALF: return bios.h[(addr - BIOS_START) >> 1];
        case WIDTH_WORD: return bios.w[(addr - BIOS_START) >> 2];
        default: printf("[BUS]\tERROR: undefined width %d\n", width);
        }
    }
    else if(IN_RANGE(CACHECTRL_START, CACHECTRL_SIZE)){

    }

    //printf("[BUS]\t warning - unhandled load  0x%08x (region adjusted 0x%08x)\n", address, addr);
    return 0;
}

void Bus::store(Uint32 address, Uint32 data, Uint8 width){
    Uint32 addr = address & mask[address >> 29];
    if(IN_RANGE(RAM_START, RAM_SIZE)){
        switch(width){
        case WIDTH_BYTE: ram.b[(addr - RAM_START)     ] = data; break;
        case WIDTH_HALF: ram.h[(addr - RAM_START) >> 1] = data; break;
        case WIDTH_WORD: ram.w[(addr - RAM_START) >> 2] = data; break;
        default: printf("[BUS]\tERROR: undefined width %d\n", width);
        }
    }
    else if(IN_RANGE(IO_START, IO_SIZE)){
      switch(addr){
        case 0x1f801000 ... 0x1f801020: // memory ctrl registers
          break;

        case 0x1f801060: // RAM_SIZE register
          break;

        case 0x1f801070 ... 0x1f801074: // I_STAT and I_MASK
          cpu->writeInterrupt(addr - 0x1f801070, data);
          break;

        case 0x1f801080 ... 0x1f8010ff: // DMA registers
          dma->write(addr - 0x1f801080, data);
          break;

        case 0x1f801100 ... 0x1f80112f: // timers
          break;

        case 0x1f801800 ... 0x1f801803: // cd drive registers
          drive->write(addr - 0x1f801800, data);
          break;

        case 0x1f801810: // GP0 register
          gpu->gp0Write(data);
          break;

        case 0x1f801814: // GP1 register
          gpu->gp1Write(data);
          break;

        case 0x1f801c00 ... 0x1f801e80: // various SPU registers
          break;

        case 0x1f802000 ... 0x1f802067: // Expansion region 2 registers;
          break;

        default:
          printf("[BUS]\tERROR: undefined IO store at 0x%08x\n", address);
      }
    }
    //else{
    //    printf("[BUS]\t warning - unhandled store 0x%08x (region adjusted 0x%08x) with data 0x%08x\n", address, addr, data);
    //}
}
