#pragma once

#include "idt.h"

using IRQHandler = void(*)(StackFrame* frame);

class IRQ
{
public:
    static void Init();
    static void RegisterHandlers(int irq, IRQHandler handler);

    static IRQHandler irqHandlers[16];
};