#include "irq.h"

#include <utils/logger.h>
#include "pic.h"

#define PIC_REMAP_OFFSET 32

IRQHandler IRQ::irqHandlers[16] = {nullptr};

void Handler(StackFrame *frame)
{
    int irq = frame->vector - PIC_REMAP_OFFSET;

    if (IRQ::irqHandlers[irq] != nullptr)
    {
        IRQ::irqHandlers[irq](frame);
    }
    else
    {
        log(WARN, "Unhandled IRQ %d...\n", irq);
    }
    
    PIC::SendEOI(irq);
}

void IRQ::Init()
{
    PIC::Init(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    for (int i = 0; i < 16; i++)
    {
        IDT::Manager::RegisterHandler(PIC_REMAP_OFFSET + i, Handler);
    }
    
    asm volatile("sti");
    log(INFO, "IRQ - Initialized!\n");
}

void IRQ::RegisterHandlers(int irq, IRQHandler handler)
{
    irqHandlers[irq] = handler;
}