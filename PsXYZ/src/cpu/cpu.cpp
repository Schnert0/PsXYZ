#include "cpu.h"

c_cpu::c_cpu(){
}

c_cpu::~c_cpu(){
}

void c_cpu::getComponents(c_mem* memPtr){
    mem = memPtr;
}

void c_cpu::reset(){
    ins.raw = 0;

    memset(r, 0, sizeof(r));
    hi = 0;
    lo = 0;

    currPC = 0xbfc00000;
    nextPC = currPC + 4;

    #ifdef DEBUG
    debug = false;
    #endif // DEBUG
}

void c_cpu::runFrame(){
    cycles = 591900;
    while(cycles > 0){
        runInstruction();
        cycles--;
    }
}

void c_cpu::runInstruction(){
    ins.raw = mem->load(currPC, WIDTH_WORD);

    currPC = nextPC;
    nextPC += 4;

    isBranch = branchDelay;
    branchDelay = false;

    r[indexSlot] = dataSlot;
    //r[indexDelay] = dataDelay;
    dataSlot = dataDelay;
    dataDelay = 0;
    indexSlot = indexDelay;
    indexDelay = 0;

    r[0] = 0;

    #ifdef DEBUG
    if(debug && !ins.raw) printf("NOP\n");
    #endif // DEBUG
    if(ins.raw){
        switch(ins.op){
        case 0x00: op_special(); break;
        case 0x01: op_BNZ();     break;
        case 0x02: op_J();       break;
        case 0x03: op_JAL();     break;
        case 0x04: op_BEQ();     break;
        case 0x05: op_BNE();     break;
        case 0x06: op_BLEZ();    break;
        case 0x07: op_BGTZ();    break;
        case 0x08: op_ADDI();    break;
        case 0x09: op_ADDIU();   break;
        case 0x0a: op_SLTI();    break;
        case 0x0b: op_SLTIU();   break;
        case 0x0c: op_ANDI();    break;
        case 0x0d: op_ORI();     break;
        case 0x0e: op_XORI();    break;
        case 0x0f: op_LUI();     break;
        case 0x10: op_COP0();    break;
        case 0x11: op_COP1();    break;
        case 0x12: op_COP2();    break;
        case 0x13: op_COP3();    break;
        case 0x20: op_LB();      break;
        case 0x21: op_LH();      break;
        case 0x22: op_LWL();     break;
        case 0x23: op_LW();      break;
        case 0x24: op_LBU();     break;
        case 0x25: op_LHU();     break;
        case 0x26: op_LWR();     break;
        case 0x28: op_SB();      break;
        case 0x29: op_SH();      break;
        case 0x2a: op_SWL();     break;
        case 0x2b: op_SW();      break;
        case 0x2e: op_SWR();     break;
        case 0x30: op_LWC0();    break;
        case 0x31: op_LWC1();    break;
        case 0x32: op_LWC2();    break;
        case 0x33: op_LWC3();    break;
        case 0x38: op_SWC0();    break;
        case 0x39: op_SWC1();    break;
        case 0x3a: op_SWC2();    break;
        case 0x3b: op_SWC3();    break;
        default:   op_ILLEGAL(); break;
        }
    }

    #ifdef DEBUG
    if(debug){
        for(int i = 0; i < 32; i++){
            printf("r%d:\t0x%08x\n", i, r[i]);
        }
        printf("\n");
        printf("HI:\t0x%08x\n", hi);
        printf("LO:\t0x%08x\n", lo);
        printf("PC:\t0x%08x\n", currPC);
        getchar();
    }
    #endif // DEBUG
}


void c_cpu::op_special(){
    switch(ins.fn){
    case 0x00: op_SLL();     break;
    case 0x02: op_SRL();     break;
    case 0x03: op_SRA();     break;
    case 0x04: op_SLLV();    break;
    case 0x06: op_SRLV();    break;
    case 0x07: op_SRAV();    break;
    case 0x08: op_JR();      break;
    case 0x09: op_JALR();    break;
    case 0x0c: op_SYSCALL(); break;
    case 0x0d: op_BREAK();   break;
    case 0x10: op_MFHI();    break;
    case 0x11: op_MTHI();    break;
    case 0x12: op_MFLO();    break;
    case 0x13: op_MTLO();    break;
    case 0x18: op_MULT();    break;
    case 0x19: op_MULTU();   break;
    case 0x1a: op_DIV();     break;
    case 0x1b: op_DIVU();    break;
    case 0x20: op_ADD();     break;
    case 0x21: op_ADDU();    break;
    case 0x22: op_SUB();     break;
    case 0x23: op_SUBU();    break;
    case 0x24: op_AND();     break;
    case 0x25: op_OR();      break;
    case 0x26: op_XOR();     break;
    case 0x27: op_NOR();     break;
    case 0x2a: op_SLT();     break;
    case 0x2b: op_SLTU();    break;
    default:   op_ILLEGAL(); break;
    }
}


void c_cpu::raiseException(Uint8 code){
    Uint32 sr = cop0.r[12];
    Uint32 cause = cop0.r[13];

    Uint32 handler;
    if(sr & (1 << 22)){
        handler = 0xbfc00180;
    }else{
        handler = 0x80000080;
    }

    Uint32 mode = sr & 0x3f;
    sr &= ~0x3f;
    sr |= (mode << 2) & 0x3f;
    cop0.r[12] = sr;

    cause = code << 2;
    cop0.r[13] = cause;

    if(isBranch){
        cop0.r[14]  = currPC - 4;
        cop0.r[13] |= 0x80000000;
    }else{
       cop0.r[14]  = currPC;
       cop0.r[13] &= 0x7fffffff;
    }

    currPC = handler;
    nextPC = currPC + 4;
}


void c_cpu::storeReg(Uint8 index, Uint32 data){
    if(index == indexSlot){
        dataSlot = 0;
        indexSlot = 0;
        printf("CPU: load delay slot overwritten\n");
    }
    r[index] = data;
}

void c_cpu::storeLoadDelay(Uint8 index, Uint32 data){
    if(index == indexSlot)
        r[index] = dataSlot;

    indexDelay = index;
    dataDelay  = data;
}

Uint32 c_cpu::loadReg(Uint8 index){
    return r[index];
}

void c_cpu::jump(){
    nextPC = (currPC & 0xf0000000) | (ins.target << 2);
    branchDelay = true;
}

void c_cpu::branch(){
    nextPC = currPC + (ins.offset << 2);
    branchDelay = true;
}
