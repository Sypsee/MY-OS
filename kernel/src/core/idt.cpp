#include "idt.h"
#include <utils/logger.h>

__attribute__((aligned(0x10)))
static IDT_Entry g_IDT[256];
static IDT_Descriptor g_IDT_Descriptor = {
    sizeof(g_IDT) - 1,
    g_IDT
};
extern void* stubs[];
ISR_Handler g_ISRHandlers[256];

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
extern "C" void _ExceptionHandler(stack_frame* frame)
{
    if (g_ISRHandlers[frame->vector] != NULL)
    {
        g_ISRHandlers[frame->vector](frame);
    }
    else if (frame->vector > 31)
    {
        log(WARN, "Unhandled exception! : %llu\n", frame->vector);
    }
    else
    {
        log(PANIC, "Exception : %llu, %s\n", frame->vector, interrupt_errors[frame->vector]);
        __asm__ volatile("cli");
        for(;;) __asm__ volatile ("hlt");
    }
}

void IDT::IDTSetGate(uint8_t interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags)
{
    g_IDT[interrupt].base_low = (uint64_t)base & 0xFFFF;
    g_IDT[interrupt].base_middle = ((uint64_t)base >> 16) & 0xFFFF;
    g_IDT[interrupt].base_high = ((uint64_t)base >> 32) & 0xFFFFFFFF;
    g_IDT[interrupt].cs = segmentDescriptor;
    g_IDT[interrupt].flags = flags;
    g_IDT[interrupt].ist = 0;
    g_IDT[interrupt].reserved = 0;
}

void IDT::IDTEnableGate(int interrupt)
{
    g_IDT[interrupt].flags |= (IDT_FLAG_PRESENT);
}

void IDT::IDTDisableGate(int interrupt)
{
    g_IDT[interrupt].flags &= ~(IDT_FLAG_PRESENT);
}

void IDT::RegisterHandler(int interrupt, ISR_Handler handler)
{
    g_ISRHandlers[interrupt] = handler;
    IDTEnableGate(interrupt);
}

void IDT::Init()
{
    for (uint8_t interrupt = 0; interrupt < 32; interrupt++)
    {
        IDTSetGate(interrupt, stubs[interrupt], 0x08, GATE_32BIT_TRAP | IDT_FLAG_PRESENT);
    }

    for (uint16_t interrupt = 32; interrupt < 256; interrupt++)
    {
        IDTSetGate(interrupt, stubs[interrupt], 0x08, GATE_32BIT_INT | IDT_FLAG_PRESENT);
    }

    IDTDisableGate(0x80); // Disable syscalls until implemented

    __asm__ volatile("lidt %0" :: "m"(g_IDT_Descriptor): "memory");
    __asm__ volatile("sti");

    log(INFO, "IDT - Initialized!, Base: %p\n", g_IDT_Descriptor.ptr);
}
