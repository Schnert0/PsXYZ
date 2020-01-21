#include "cpu.h"

void c_cpu::op_ILLEGAL(){
    if(!ins.op)
        printf("CPU: error - unidentified special opcode 0x%02x at 0x%08x\n", ins.fn, currPC);
    else
        printf("CPU: error - unidentified opcode 0x%02x at 0x%08x\n", ins.op, currPC);
    getchar();
}


void c_cpu::op_LUI(){
    storeReg(ins.rt, ins.imm << 16);

    #ifdef DEBUG
        if(debug) printf("LUI\t$%d, 0x%04x\n", ins.rt, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_ORI(){
    storeReg(ins.rt, loadReg(ins.rs) | ins.imm);

    #ifdef DEBUG
        if(debug) printf("ORI\t$%d, $%d 0x%04x\n", ins.rt, ins.rs, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_SW(){
    if(!(cop0.r[12] & 0x10000))
        mem->store(loadReg(ins.rs)+ins.offset, loadReg(ins.rt), WIDTH_WORD);

    #ifdef DEBUG
        if(debug) printf("SW\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_SLL(){
    storeReg(ins.rd, loadReg(ins.rt) << ins.sh);

    #ifdef DEBUG
        if(debug) printf("SLL\t$%d, %d\n", ins.rt, ins.sh);
    #endif // DEBUG
}


void c_cpu::op_ADDIU(){
    storeReg(ins.rt, loadReg(ins.rs) + ins.offset);

    #ifdef DEBUG
        if(debug) printf("ADDIU\t$%d, $%d 0x%04x\n", ins.rt, ins.rs, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_J(){
    jump();

    #ifdef DEBUG
        if(debug) printf("J\t0x%08x\n", nextPC);
    #endif // DEBUG
}


void c_cpu::op_OR(){
    storeReg(ins.rd, loadReg(ins.rs) | loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("OR\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_COP0(){
    switch(ins.rs){
        case 0x00: storeLoadDelay(ins.rt, cop0.r[ins.rd]); break;
        case 0x04: cop0.r[ins.rd] = loadReg(ins.rt); break;
        case 0x10:
            switch(ins.fn){
            case 0x10: op_RFE(); break;
            default: printf("undefined special COP0 opcode 0x%02x at 0x%08x\n", ins.fn, currPC); getchar();
            }
            break;
        default: printf("CPU: error - undefined COP0 opcode 0x%02x at 0x%08x\n", ins.rs, currPC); getchar();
    }

    #ifdef DEBUG
    if(debug){
        switch(ins.rs){
        case 0x04: printf("MTC0\tcop0_$%d, $%d\n", ins.rd, ins.rt); break;
        }
    }
    #endif // DEBUG
}


void c_cpu::op_BNE(){
    if(loadReg(ins.rs) != loadReg(ins.rt))
        branch();

    #ifdef DEBUG
        if(debug) printf("BNE\t$%d, $%d 0x%08x\n", ins.rs, ins.rt, nextPC);
    #endif // DEBUG
}


void c_cpu::op_ADDI(){
    storeReg(ins.rt, loadReg(ins.rs) + ins.offset);

    #ifdef DEBUG
        if(debug) printf("ADDI\t$%d, $%d 0x%04x\n", ins.rt, ins.rs, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_LW(){
    if(!(cop0.r[12] & 0x10000))
        storeLoadDelay(ins.rt, mem->load(loadReg(ins.rs) + ins.offset, WIDTH_WORD));

    #ifdef DEBUG
        if(debug) printf("LW\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_SLTU(){
    storeReg(ins.rd, loadReg(ins.rs) < loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("SLTU\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_ADDU(){
    storeReg(ins.rd, loadReg(ins.rs) + loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("ADDU\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_SH(){
    if(!(cop0.r[12] & 0x10000))
        mem->store(loadReg(ins.rs)+ins.offset, loadReg(ins.rt), WIDTH_HALF);

    #ifdef DEBUG
        if(debug) printf("SH\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_JAL(){
    storeReg(31, currPC+4);
    jump();

    #ifdef DEBUG
        if(debug) printf("JAL\t0x%08x\n", nextPC);
    #endif // DEBUG
}


void c_cpu::op_ANDI(){
    storeReg(ins.rt, loadReg(ins.rs) & ins.imm);

    #ifdef DEBUG
        if(debug) printf("ANDI\t$%d, $%d 0x%04x\n", ins.rt, ins.rs, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_SB(){
    if(!(cop0.r[12] & 0x10000))
        mem->store(loadReg(ins.rs)+ins.offset, loadReg(ins.rt), WIDTH_BYTE);

    #ifdef DEBUG
        if(debug) printf("SB\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_JR(){
    nextPC = loadReg(ins.rs);

    #ifdef DEBUG
        if(debug) printf("J\t0x%08x\n", nextPC);
    #endif // DEBUG
}


void c_cpu::op_LB(){
    if(!(cop0.r[12] & 0x10000))
        storeLoadDelay(ins.rt, (Sint32)(Sint8)mem->load(loadReg(ins.rs) + ins.offset, WIDTH_BYTE));

    #ifdef DEBUG
        if(debug) printf("LB\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_BEQ(){
    if(loadReg(ins.rs) == loadReg(ins.rt))
        branch();

    #ifdef DEBUG
        if(debug) printf("BEQ\t$%d, $%d 0x%08x\n", ins.rs, ins.rt, nextPC);
    #endif // DEBUG
}


void c_cpu::op_AND(){
    storeReg(ins.rd, loadReg(ins.rs) & loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("AND\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_ADD(){
    storeReg(ins.rd, loadReg(ins.rs) + loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("ADD\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_BGTZ(){
    if((Sint32)loadReg(ins.rs) > 0)
        branch();

    #ifdef DEBUG
        if(debug) printf("BGTZ\t$%d, 0x%08x\n", ins.rs, nextPC);
    #endif // DEBUG
}


void c_cpu::op_BLEZ(){
    if((Sint32)loadReg(ins.rs) <= 0)
        branch();

    #ifdef DEBUG
        if(debug) printf("BLEZ\t$%d, 0x%08x\n", ins.rs, nextPC);
    #endif // DEBUG
}


void c_cpu::op_LBU(){
    if(!(cop0.r[12] & 0x10000))
        storeLoadDelay(ins.rt, mem->load(loadReg(ins.rs) + ins.offset, WIDTH_BYTE));

    #ifdef DEBUG
        if(debug) printf("LBU\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_JALR(){
    storeReg(ins.rd, currPC+4);
    nextPC = loadReg(ins.rs);
}


void c_cpu::op_BNZ(){
    bool GE   = (ins.rt & 0x01);
    bool link = (ins.rt & 0x1e) == 0x10;
    bool cond;

    if(GE){
        cond = (Sint32)loadReg(ins.rs) >= 0;
    }
    else{
        cond = (Sint32)loadReg(ins.rs) < 0;
    }

    if(cond){
        if(link) storeReg(31, currPC+4);
        branch();
    }

    #ifdef DEBUG
    if(debug){
        if( GE && !link)
              printf("BGEZ\t $%d, 0x%08x\n", ins.rs, nextPC);
        if(!GE && !link)
              printf("BLTZ\t $%d, 0x%08x\n", ins.rs, nextPC);
        if( GE &&  link)
            printf("BGEZAL\t $%d, 0x%08x\n", ins.rs, nextPC);
        if(!GE &&  link)
            printf("BLTZAL\t $%d, 0x%08x\n", ins.rs, nextPC);
    }
    #endif // DEBUG
}


void c_cpu::op_SLTI(){
    storeReg(ins.rt, (Sint32)loadReg(ins.rs) < (Sint32)ins.offset);

    #ifdef DEBUG
        if(debug) printf("SLTI\t$%d, $%d, 0x%04x\n", ins.rt, ins.rs, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_SUBU(){
    storeReg(ins.rd, loadReg(ins.rs) - loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("SUBU\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_SRA(){
    storeReg(ins.rd, (Sint32)loadReg(ins.rt) >> ins.sh);

    #ifdef DEBUG
        if(debug) printf("SRA\t$%d, %d\n", ins.rt, ins.sh);
    #endif // DEBUG
}


void c_cpu::op_DIV(){
    Sint32 a = (Sint32)loadReg(ins.rs);
    Sint32 b = (Sint32)loadReg(ins.rt);

    if(b == 0){
        hi = a;
        if(b < 0)
            lo = 0x00000001;
        else
            lo = 0xffffffff;
    }
    else if(a == -1 && b == (Sint32)0x80000000){
        hi = 0;
        lo = 0x80000000;
    }
    else{
        hi = a % b;
        lo = a / b;
    }

    #ifdef DEBUG
        if(debug) printf("DIV\t$%d, $%d\n", ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_MFLO(){
    storeReg(ins.rd, lo);

    #ifdef DEBUG
        if(debug) printf("MFLO\t$%d\n", ins.rd);
    #endif // DEBUG
}


void c_cpu::op_SRL(){
    storeReg(ins.rd, loadReg(ins.rt) >> ins.sh);

    #ifdef DEBUG
        if(debug) printf("SRL\t$%d, %d\n", ins.rt, ins.sh);
    #endif // DEBUG
}


void c_cpu::op_SLTIU(){
    storeReg(ins.rt, loadReg(ins.rs) < (Uint32)ins.offset);

    #ifdef DEBUG
        if(debug) printf("SLTIU\t$%d, $%d, 0x%04x\n", ins.rt, ins.rs, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_DIVU(){
    Uint32 a = loadReg(ins.rs);
    Uint32 b = loadReg(ins.rt);

    if(b == 0){
        hi = a;
        lo = 0xffffffff;
    }
    else{
        hi = a % b;
        lo = a / b;
    }

    #ifdef DEBUG
        if(debug) printf("DIVU\t$%d, $%d\n", ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_MFHI(){
    storeReg(ins.rd, hi);

    #ifdef DEBUG
        if(debug) printf("MFHI\t$%d\n", ins.rd);
    #endif // DEBUG
}

void c_cpu::op_SLT(){
    storeReg(ins.rd, (Sint32)loadReg(ins.rs) < (Sint32)loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("SLT\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_SYSCALL(){
    raiseException(EXCEPTION_SYSCALL);
}


void c_cpu::op_MTLO(){
    lo = loadReg(ins.rs);

    #ifdef DEBUG
        if(debug) printf("MTLO\t$%d\n", ins.rs);
    #endif // DEBUG
}


void c_cpu::op_MTHI(){
    hi = loadReg(ins.rs);

    #ifdef DEBUG
        if(debug) printf("MTHI\t$%d\n", ins.rs);
    #endif // DEBUG
}


void c_cpu::op_RFE(){
    Uint32 sr = cop0.r[12];

    Uint32 mode = sr & 0x3f;
    sr &= ~0x3f;
    sr |= mode >> 2;

    cop0.r[12] = sr;
}


void c_cpu::op_LHU(){
    if(!(cop0.r[12] & 0x10000))
        storeLoadDelay(ins.rt, mem->load(loadReg(ins.rs) + ins.offset, WIDTH_HALF));

    #ifdef DEBUG
        if(debug) printf("LHU\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}

void c_cpu::op_SLLV(){
    storeReg(ins.rd, loadReg(ins.rt) << (loadReg(ins.rs) & 0x1f));

    #ifdef DEBUG
        if(debug) printf("SLLV\t$%d, %d\n", ins.rt, ins.sh);
    #endif // DEBUG
}


void c_cpu::op_LH(){
    if(!(cop0.r[12] & 0x10000))
        storeLoadDelay(ins.rt, (Sint32)(Sint16)mem->load(loadReg(ins.rs) + ins.offset, WIDTH_HALF));

    #ifdef DEBUG
        if(debug) printf("LH\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_NOR(){
   storeReg(ins.rd, ~(loadReg(ins.rs) | loadReg(ins.rt)));

    #ifdef DEBUG
        if(debug) printf("NOR\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_SRAV(){
    storeReg(ins.rd, (Sint32)loadReg(ins.rt) >> (loadReg(ins.rs) & 0x1f));

    #ifdef DEBUG
        if(debug) printf("SRAV\t$%d, %d\n", ins.rt, ins.sh);
    #endif // DEBUG
}


void c_cpu::op_SRLV(){
    storeReg(ins.rd, loadReg(ins.rt) >> (loadReg(ins.rs) & 0x1f));

    #ifdef DEBUG
        if(debug) printf("SRLV\t$%d, %d\n", ins.rt, ins.sh);
    #endif // DEBUG
}


void c_cpu::op_MULTU(){
    Uint64 a = loadReg(ins.rs);
    Uint64 b = loadReg(ins.rt);

    Uint64 c = a * b;

    hi = c >> 32;
    lo = c & 0xffffffff;

    #ifdef DEBUG
        if(debug) printf("MULTU $%d, $%d", ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_XOR(){
    storeReg(ins.rd, loadReg(ins.rs) ^ loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("XOR\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_BREAK(){
    raiseException(EXCEPTION_BREAK);

    #ifdef DEBUG
        if(debug) printf("BREAK\n");
    #endif // DEBUG
}


void c_cpu::op_MULT(){
    Sint64 a = (Sint64)(Sint32)loadReg(ins.rs);
    Sint64 b = (Sint64)(Sint32)loadReg(ins.rt);

    Sint64 c = a * b;

    hi = c >> 32;
    lo = c & 0xffffffff;

    #ifdef DEBUG
        if(debug) printf("MULT $%d, $%d", ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_SUB(){
    storeReg(ins.rd, loadReg(ins.rs) - loadReg(ins.rt));

    #ifdef DEBUG
        if(debug) printf("SUB\t$%d, $%d $%d\n", ins.rd, ins.rs, ins.rt);
    #endif // DEBUG
}


void c_cpu::op_XORI(){
    storeReg(ins.rt, loadReg(ins.rs) ^ ins.imm);

    #ifdef DEBUG
        if(debug) printf("XORI\t$%d, $%d 0x%04x\n", ins.rt, ins.rs, ins.imm);
    #endif // DEBUG
}


void c_cpu::op_COP1(){
    raiseException(EXCEPTION_COPROCESSOR);
}


void c_cpu::op_COP2(){
    switch(ins.rs){
    default: printf("COP2: error - undefined COP2 opcode 0x%02x at 0x%02x\n", ins.rs, currPC); getchar();
    }
}


void c_cpu::op_COP3(){
    raiseException(EXCEPTION_COPROCESSOR);
}


void c_cpu::op_LWL(){
    Uint32 addr = loadReg(ins.rs) + ins.offset;
    Uint32 data = mem->load(addr & 0xfffffffc, WIDTH_WORD);

    Uint32 reg;
    if(indexDelay == ins.rt)
        reg = dataDelay;
    else
        reg = loadReg(ins.rt);

    Uint32 result = 0;
    switch(addr & 3){
    case 0: result = (reg & 0x00ffffff) | (data << 24);
    case 1: result = (reg & 0x0000ffff) | (data << 16);
    case 2: result = (reg & 0x000000ff) | (data <<  8);
    case 3: result = (reg & 0x00000000) | (data      );
    }
    storeLoadDelay(ins.rt, result);

    #ifdef DEBUG
        if(debug) printf("LWL\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_LWR(){
    Uint32 addr = loadReg(ins.rs) + ins.offset;
    Uint32 data = mem->load(addr & 0xfffffffc, WIDTH_WORD);

    Uint32 reg;
    if(indexSlot == ins.rt)
        reg = dataSlot;
    else
        reg = loadReg(ins.rt);

    Uint32 result = 0;
    switch(addr & 3){
    case 0: result = (reg & 0x00000000) | (data      );
    case 1: result = (reg & 0xff000000) | (data >>  8);
    case 2: result = (reg & 0xffff0000) | (data >> 16);
    case 3: result = (reg & 0xffffff00) | (data >> 24);
    }
    storeLoadDelay(ins.rt, result);

    #ifdef DEBUG
        if(debug) printf("LWR\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_SWL(){
    Uint32 addr = loadReg(ins.rs) + ins.offset;
    Uint32 data = mem->load(addr & 0xfffffffc, WIDTH_WORD);

    Uint32 reg = loadReg(ins.rt);

    Uint32 result = 0;
    switch(addr & 3){
    case 0: result = (reg & 0xffffff00) | (data >> 24);
    case 1: result = (reg & 0xffff0000) | (data >> 16);
    case 2: result = (reg & 0xff000000) | (data >>  8);
    case 3: result = (reg & 0x00000000) | (data      );
    }
    mem->store(addr & 0xfffffffc, result, WIDTH_WORD);

    #ifdef DEBUG
        if(debug) printf("LWL\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_SWR(){
    Uint32 addr = loadReg(ins.rs) + ins.offset;
    Uint32 data = mem->load(addr & 0xfffffffc, WIDTH_WORD);

    Uint32 reg = loadReg(ins.rt);

    Uint32 result = 0;
    switch(addr & 3){
    case 0: result = (reg & 0x00000000) | (data      );
    case 1: result = (reg & 0x000000ff) | (data <<  8);
    case 2: result = (reg & 0x0000ffff) | (data << 16);
    case 3: result = (reg & 0x00ffffff) | (data << 24);
    }
    mem->store(addr & 0xfffffffc, result, WIDTH_WORD);

    #ifdef DEBUG
        if(debug) printf("LWL\t$%d, 0x%04x($%d)\n", ins.rt, ins.imm, ins.rs);
    #endif // DEBUG
}


void c_cpu::op_LWC0(){
    raiseException(EXCEPTION_COPROCESSOR);
}


void c_cpu::op_LWC1(){
    raiseException(EXCEPTION_COPROCESSOR);
}


void c_cpu::op_LWC2(){
    printf("CPU: error - unhandled GTE LWC2\n");
    getchar();
}


void c_cpu::op_LWC3(){
    raiseException(EXCEPTION_COPROCESSOR);
}


void c_cpu::op_SWC0(){
    raiseException(EXCEPTION_COPROCESSOR);
}


void c_cpu::op_SWC1(){
    raiseException(EXCEPTION_COPROCESSOR);
}


void c_cpu::op_SWC2(){
    printf("CPU: error - unhandled GTE SWC2\n");
    getchar();
}


void c_cpu::op_SWC3(){
    raiseException(EXCEPTION_COPROCESSOR);
}
