// hw_regs.h
#ifndef HW_REGS_H
#define HW_REGS_H

#define REG32(addr)   (*(volatile int *)(addr))
#define REG16(addr)   (*(volatile short *)(addr))
#define MMIO8(addr)   ((volatile unsigned char *)(addr))
#define MMIO32(addr)  ((volatile unsigned int  *)(addr))

// Timer
#define TMR1_SR       REG16(0x04000020)
#define TMR1_CR       REG16(0x04000024)
#define TMR1_PERLO    REG16(0x04000028)
#define TMR1_PERHI    REG16(0x0400002C)
#define TMR1_SNAPLO   REG16(0x04000030)

// LEDs
#define LEDS          REG32(0x04000000)

// Switches
#define SW            REG32(0x04000010)
#define SW_DIR        REG32(0x04000014)
#define SW_IRQ        REG32(0x04000018)
#define SW_EDGE       REG32(0x0400001C)

// Button
#define BTN           REG32(0x040000D0)
#define BTN_IRQ       REG32(0x040000D8)
#define BTN_EDGE      REG32(0x040000DC)

// 7-seg
#define SEVSEG_BASE   0x04000050u

// VGA-display
#define VGA_FB        MMIO8(0x08000000)
#define VGA_CTRL      MMIO32(0x04000100)

#define REG_BUFFER    0
#define REG_BACK      1
#define REG_RES       2
#define REG_STATUS    3

#endif // HW_REGS_H