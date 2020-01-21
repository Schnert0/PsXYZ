#include "system.h"

c_system::c_system(){
    gpu = new c_gpu;
    mem = new c_mem;
    dma = new c_dma;
    cpu = new c_cpu;

    mem->getComponents(gpu, dma);
    dma->getComponents(gpu, mem);
    cpu->getComponents(mem);
}


c_system::~c_system(){
}


void c_system::reset(){
    gpu->reset();
    dma->reset();
    mem->reset();
    cpu->reset();
}


void c_system::loadBIOS(const char* path){
    mem->loadBIOS(path);
}


void c_system::run(){
    cpu->runFrame();
}
