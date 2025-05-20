#pragma once

#include "idt.h"

using IRQ_Handler = void(*)(stack_frame* frame);

class IRQ
{
public:
    static void Init();
    static void RegisterHandlers(int irq, IRQ_Handler frame);
};