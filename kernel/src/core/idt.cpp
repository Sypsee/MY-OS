#include "idt.h"
#include <utils/logger.h>

extern void* stubs[];
static const char *interrupt_errors[32] = {
    "Division by Zero",
    "Debug",
    "Non-Maskable-Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid opcode",
    "Device (FPU) not available",
    "Double Fault",
    "RESERVED VECTOR",
    "Invalid TSS",
    "Segment not present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault ",
    "RESERVED VECTOR",
    "x87 FP Exception",
    "Alignment Check",
    "Machine Check (Internal Error)",
    "SIMD FP Exception",
    "Virtualization Exception",
    "Control  Protection Exception",
    "RESERVED VECTOR",
    "RESERVED VECTOR",
    "RESERVED VECTOR",
    "RESERVED VECTOR",
    "RESERVED VECTOR",
    "RESERVED VECTOR",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "RESERVED VECTOR"
};

__attribute__((noreturn))
extern "C" void _ExceptionHandler(StackFrame* frame)
{
    if (IDT::Manager::handlers[frame->vector] != NULL)
    {
        IDT::Manager::handlers[frame->vector](frame);
    }
    else if (frame->vector > 31)
    {
        log(WARN, "Unhandled exception! : %llu\n", frame->vector);
    }
    else
    {
        log(PANIC, "Exception : %llu, %s\n", frame->vector, interrupt_errors[frame->vector]);
        asm volatile("cli");
        for(;;) asm volatile ("hlt");
    }
}

namespace IDT
{
void Manager::SetGate(uint8_t interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags)
{
    idt[interrupt].base_low = (uint64_t)base & 0xFFFF;
    idt[interrupt].base_middle = ((uint64_t)base >> 16) & 0xFFFF;
    idt[interrupt].base_high = ((uint64_t)base >> 32) & 0xFFFFFFFF;
    idt[interrupt].cs = segmentDescriptor;
    idt[interrupt].flags = flags;
    idt[interrupt].ist = 0;
    idt[interrupt].reserved = 0;
}

void Manager::EnableGate(int interrupt)
{
    idt[interrupt].flags |= (Flags::Present);
}

void Manager::DisableGate(int interrupt)
{
    idt[interrupt].flags &= ~(Flags::Present);
}

void Manager::RegisterHandler(int interrupt, ISRHandler handler)
{
    handlers[interrupt] = handler;
    EnableGate(interrupt);
}

void Manager::Init()
{
    for (uint8_t interrupt = 0; interrupt < 32; interrupt++)
    {
        SetGate(interrupt, stubs[interrupt], 0x08, Flags::Gate32BitTrap | Flags::Present);
    }

    for (uint16_t interrupt = 32; interrupt < 256; interrupt++)
    {
        SetGate(interrupt, stubs[interrupt], 0x08, Flags::Gate32BitInt | Flags::Present);
    }

    DisableGate(0x80); // Disable syscalls until implemented

    asm volatile("lidt %0" :: "m"(descriptor): "memory");
    asm volatile("sti");

    log(INFO, "IDT - Initialized!, Base: %p\n", descriptor.ptr);
}
};