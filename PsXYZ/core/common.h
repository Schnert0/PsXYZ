#ifndef common_h
#define common_h

//#define DEBUG

#define KiB(size) size*1024

#define RAM_START        0x00000000
#define EXP1_START       0x1f000000
#define SCRATCHPAD_START 0x1f800000
#define IO_START         0x1f801000
#define BIOS_START       0x1fc00000
#define CACHECTRL_START  0xfffe0000

#define RAM_SIZE        KiB(2048)
#define EXP1_SIZE       KiB(8192)
#define SCRATCHPAD_SIZE KiB(1)
#define IO_SIZE         KiB(8)
#define BIOS_SIZE       KiB(512)
#define CACHECTRL_SIZE  512

#define IN_RANGE(start, size) addr >= start && addr < (start+size)

enum{
    WIDTH_BYTE,
    WIDTH_HALF,
    WIDTH_WORD
};

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

enum{
  IRQ_VBLANK     = 0,
  IRQ_GPU        = 1,
  IRQ_CDROM      = 2,
  IRQ_DMA        = 3,
  IRQ_TMR0       = 4,
  IRQ_TMR1       = 5,
  IRQ_TMR2       = 6,
  IRQ_CONTROLLER = 7,
  IRQ_SIO        = 8,
  IRQ_SPU        = 9,
  IRQ_LIGHTPEN   = 10
};

#endif // common_h
