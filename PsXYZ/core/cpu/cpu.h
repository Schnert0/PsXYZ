#ifndef cpu_h
#define cpu_h

#include <SDL.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "../bus/bus.h"

enum{
    EXCEPTION_INTERRUPT   = 0x00,
    EXCEPTION_LOADADDR    = 0x04,
    EXCEPTION_STOREADDR   = 0x05,
    EXCEPTION_SYSCALL     = 0x08,
    EXCEPTION_BREAK       = 0x09,
    EXCEPTION_ILLEGAL_OP  = 0x0a,
    EXCEPTION_COPROCESSOR = 0x0b,
    EXCEPTION_OVERFLOW    = 0x0c
};

class Bus;

class CPU{
private:
    Bus* bus;

    int cycles;

    union{
      struct {
        Uint32 fn : 6;
        Uint32 sh : 5;
        Uint32 rd : 5;
        Uint32 rt : 5;
        Uint32 rs : 5;
        Uint32 op : 6;
      };
      Uint32 raw;
      Uint32 target : 26;
      Uint16 imm;
      Sint16 offset;
    }ins;

    Uint32 r[32];
    Uint32 hi,lo;

    Uint32 currPC;
    Uint32 nextPC;

    bool branchDelay;
    bool isBranch;

    Uint32 dataDelay;
    Uint32 dataSlot;
    Uint8  indexDelay;
    Uint8  indexSlot;

    Uint32 istat, imask;

    struct{
      Uint32 r[16];
    }cop0;

    #ifdef DEBUG
    bool debug;
    #endif // DEBUG

    void runInstruction();

    void raiseException(Uint8 code);

    void storeReg(Uint8 index, Uint32 data);
    void storeLoadDelay(Uint8 index, Uint32 data);
    Uint32 loadReg(Uint8 index);
    void jump();
    void branch();

    void op_special();
    void op_ILLEGAL();
    void op_LUI();
    void op_ORI();
    void op_SW();
    void op_SLL();
    void op_ADDIU();
    void op_J();
    void op_OR();
    void op_COP0();
    void op_BNE();
    void op_ADDI();
    void op_LW();
    void op_SLTU();
    void op_ADDU();
    void op_SH();
    void op_JAL();
    void op_ANDI();
    void op_SB();
    void op_JR();
    void op_LB();
    void op_BEQ();
    void op_AND();
    void op_ADD();
    void op_BGTZ();
    void op_BLEZ();
    void op_LBU();
    void op_JALR();
    void op_BNZ();
    void op_SLTI();
    void op_SUBU();
    void op_SRA();
    void op_DIV();
    void op_MFLO();
    void op_SRL();
    void op_SLTIU();
    void op_DIVU();
    void op_MFHI();
    void op_SLT();
    void op_SYSCALL();
    void op_MTLO();
    void op_MTHI();
    void op_RFE();
    void op_LHU();
    void op_SLLV();
    void op_LH();
    void op_NOR();
    void op_SRAV();
    void op_SRLV();
    void op_MULTU();
    void op_XOR();
    void op_BREAK();
    void op_MULT();
    void op_SUB();
    void op_XORI();
    void op_COP1();
    void op_COP2();
    void op_COP3();
    void op_LWL();
    void op_LWR();
    void op_SWL();
    void op_SWR();
    void op_LWC0();
    void op_LWC1();
    void op_LWC2();
    void op_LWC3();
    void op_SWC0();
    void op_SWC1();
    void op_SWC2();
    void op_SWC3();

public:
     CPU();
    ~CPU();
    void getComponents(Bus* busPtr);

    void reset();
    void runFrame();

    Uint32 readInterrupt(Uint32 offset);
    void  writeInterrupt(Uint32 offset, Uint32 data);
};

#endif // cpu_h
