#pragma once

#include <stdint.h>

enum PIC_IO_Base : uint8_t {
    PIC1 = 0x20,
    PIC2 = 0xA0
};

enum PIC_Command_Data : uint8_t {
    PIC1_COMMAND = PIC1,
    PIC1_DATA = PIC1 + 1,
    PIC2_COMMAND = PIC2,
    PIC2_DATA = PIC2 + 1,
    PIC_EOI = 0x20
};

enum ICW1_Flags : uint8_t {
    ICW1_ICW4 = 0x01,
    ICW1_SINGLE = 0x02,
    ICW1_INTERVAL4 = 0x04,
    ICW1_LEVEL = 0x08,
    ICW1_INIT = 0x10
};

enum ICW4_Flags : uint8_t {
    ICW4_8086 = 0x01,
    ICW4_AUTO = 0x02,
    ICW4_BUF_SLAVE = 0x08,
    ICW4_BUF_MASTER = 0x0C,
    ICW4_SFNM = 0x10
};

enum PIC_Read_Cmd : uint8_t {
    PIC_READ_IRR = 0x0a,
    PIC_READ_ISR = 0x0b
};

class PIC
{
public:
    static void Init(uint8_t offset1, uint8_t offset2);
    static void Mask(uint8_t irq);
    static void Unmask(uint8_t irq);
    static void Disable();
    static void SendEOI(uint8_t irq);
    static uint16_t GetIRR();
    static uint16_t GetISR();
};