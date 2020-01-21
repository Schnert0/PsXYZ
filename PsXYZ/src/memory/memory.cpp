#include "memory.h"

c_mem::c_mem(){
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

c_mem::~c_mem(){
}

void c_mem::getComponents(c_gpu* gpuPtr, c_dma* dmaPtr){
    gpu = gpuPtr;
    dma = dmaPtr;
}

void c_mem::reset(){
    memset(ram.b, 0, RAM_SIZE);
}

void c_mem::loadBIOS(const char* path){
    FILE* file = fopen(path, "rb");
    if(!file){
        printf("MEM: error - unable to load \"%s\"\n", path);
        getchar();
    }
    fseek(file, 0, SEEK_END);
    Uint32 fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("BIOS is %d bytes\n", fileSize);

    if(fileSize == 524288){
        fread(bios.b, BIOS_SIZE, sizeof(Uint8), file);
    }else{
        printf("MEM: error - incorrect file size for BIOS\n");
    }
    fclose(file);
    printf("MEM: BIOS loaded successfully\n");
}

Uint32 c_mem::load(Uint32 address, Uint8 width){
    Uint32 addr = address & mask[address >> 29];
    if(IN_RANGE(RAM_START, RAM_SIZE)){
        switch(width){
        case WIDTH_BYTE: return ram.b[(addr - RAM_START)     ];
        case WIDTH_HALF: return ram.h[(addr - RAM_START) >> 1];
        case WIDTH_WORD: return ram.w[(addr - RAM_START) >> 2];
        default: printf("MEM: error - undefined width %d\n", width); getchar();
        }
    }
    else if(IN_RANGE(EXP1_START, EXP1_SIZE)){
        switch(width){
        case WIDTH_BYTE: return 0xff;
        case WIDTH_HALF: return 0xffff;
        case WIDTH_WORD: return 0xffffffff;
        default: printf("MEM: error - undefined width %d\n", width); getchar();
        }
    }
    else if(IN_RANGE(IO_START, IO_SIZE)){
        if(addr >= 0x1f801080 && addr < 0x1f801100){
            return dma->read(addr - 0x1f801080);
        }
        else if(addr == 0x1f801814){
            return 0x1c000000;
        }
        else if(addr >= 0x1f801c00 && addr < 0x1f801dbc){
            return 0;
        }
        //printf("MEM: warning - unhandled IO load 0x%08x\n", addr);
        return 0;
    }
    else if(IN_RANGE(BIOS_START, BIOS_SIZE)){
        switch(width){
        case WIDTH_BYTE: return bios.b[(addr - BIOS_START)     ];
        case WIDTH_HALF: return bios.h[(addr - BIOS_START) >> 1];
        case WIDTH_WORD: return bios.w[(addr - BIOS_START) >> 2];
        default: printf("MEM: error - undefined width %d\n", width); getchar();
        }
    }

    printf("MEM: warning - unhandled load  0x%08x (region adjusted 0x%08x)\n", address, addr);
    //getchar();
    return 0;
}

void c_mem::store(Uint32 address, Uint32 data, Uint8 width){
    Uint32 addr = address & mask[address >> 29];
    if(IN_RANGE(RAM_START, RAM_SIZE)){
        switch(width){
        case WIDTH_BYTE: ram.b[(addr - RAM_START)     ] = data; break;
        case WIDTH_HALF: ram.h[(addr - RAM_START) >> 1] = data; break;
        case WIDTH_WORD: ram.w[(addr - RAM_START) >> 2] = data; break;
        default: printf("MEM: error - undefined width %d\n", width); getchar();
        }
    }
    else if(IN_RANGE(IO_START, IO_SIZE)){
        if(addr >= 0x1f801080 && addr < 0x1f801100){
            dma->write(addr - 0x1f801080, data);
        }
        else if(addr == 0x1f801810){
            gpu->gp0Write(data);
        }
        else if(addr >= 0x1f801c00 && addr < 0x1f801dbc){
        }
        //else{
        //    printf("MEM: warning - unhandled IO write 0x%08x\n", addr);
        //}
    }
    else{
        printf("MEM: warning - unhandled store 0x%08x (region adjusted 0x%08x) with data 0x%08x\n", address, addr, data);
    }
}
