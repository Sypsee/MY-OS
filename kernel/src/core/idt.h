#pragma once

#include <stdint.h>

struct [[gnu::packed]] StackFrame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t vector, err;
    uint64_t rip, cs, rflags, rsp, ss;
};

using ISRHandler = void(*)(StackFrame* frame);

namespace IDT {
struct [[gnu::packed]] Entry {
    uint16_t base_low;
    uint16_t cs;
    uint8_t ist;
    uint8_t flags;
    uint16_t base_middle;
    uint32_t base_high;
    uint32_t reserved;
};

struct [[gnu::packed]] Descriptor {
    uint16_t limit;
    Entry *ptr;
};

enum Flags : uint8_t {
    Gate16BitInt  = 0x6,
    Gate16BitTrap = 0x7,
    Gate32BitInt  = 0xE,
    Gate32BitTrap = 0xF,

    Ring0 = 0 << 5,
    Ring1 = 1 << 5,
    Ring2 = 2 << 5,
    Ring3 = 3 << 5,

    Present = 0x80,
};

class Manager {
public:
    static void Init();
    static void SetGate(uint8_t interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags);
    static void EnableGate(int interrupt);
    static void DisableGate(int interrupt);
    static void RegisterHandler(int interrupt, ISRHandler handler);

    static inline ISRHandler handlers[256] = { nullptr };
private:
    alignas(16) static inline Entry idt[256];
    static inline Descriptor descriptor { sizeof(idt) - 1, idt };
};
}

extern "C" void _ExceptionHandler(StackFrame* frame);