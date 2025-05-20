#include "irq.h"

#include <utils/logger.h>
#include "pic.h"

#define PIC_REMAP_OFFSET 32

IRQ_Handler g_IRQHandlers[16];

void IRQHandler(stack_frame *frame)
{
    int irq = frame->vector - PIC_REMAP_OFFSET;

    if (g_IRQHandlers[irq] != nullptr)
        g_IRQHandlers[irq](frame);
    else
        log(WARN, "Unhandled IRQ %d...\n", irq);
    
    PIC::SendEOI(irq);
}

void IRQ::Init()
{
    PIC::Init(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    for (int i = 0; i < 16; i++)
        IDT::RegisterHandler(PIC_REMAP_OFFSET + i, IRQHandler);
    
    __asm__ volatile("sti");
    log(INFO, "IRQ - Initialized!\n");
}

void IRQ::RegisterHandlers(int irq, IRQ_Handler handler)
{
    g_IRQHandlers[irq] = handler;
}