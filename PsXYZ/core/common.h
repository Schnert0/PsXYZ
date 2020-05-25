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

#endif // common_h
