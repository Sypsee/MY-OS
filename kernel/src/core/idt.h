#pragma once

#include <stdint.h>

struct __attribute__((packed)) IDT_Entry
{
    uint16_t base_low;
    uint16_t cs;
    uint8_t ist;
    uint8_t flags;
    uint16_t base_middle;
    uint32_t base_high;
    uint32_t reserved;
};

struct __attribute__((packed)) IDT_Descriptor
{
    uint16_t limit;
    IDT_Entry *ptr;
};

enum IDT_Flags
{
    GATE_TASK = 0x05,
    GATE_16BIT_INT = 0x6,
    GATE_16BIT_TRAP = 0x7,
    GATE_32BIT_INT = 0xE,
    GATE_32BIT_TRAP = 0xF,

    IDT_FLAG_RING0 = 0 << 5,
    IDT_FLAG_RING1 = 1 << 5,
    IDT_FLAG_RING2 = 2 << 5,
    IDT_FLAG_RING3 = 3 << 5,

    IDT_FLAG_PRESENT = 0x80,
};

struct __attribute__((packed)) register_ctx
{
    uint64_t es, ds;
    uint64_t cr4, cr3, cr2, cr0;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
    uint64_t vector, err;
    uint64_t rip, cs, rflags, rsp, ss;
};

extern "C" void exception_handler(uint64_t interrupt);

void IDT_SetGate(uint8_t interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags);
void IDT_Enable_Gate(int interrupt);
void IDT_Disable_Gate(int interrupt);
void setup_idt();