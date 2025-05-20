#pragma once

#include <stdint.h>

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