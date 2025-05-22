#include "pic.h"

#include "io.h"
#include <utils/logger.h>

void PIC::Init(uint8_t offset1, uint8_t offset2)
{
    IO::OutB(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // start the init sequence in cascade mode
    IO::Wait();
    IO::OutB(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::Wait();

    IO::OutB(PIC1_DATA, offset1); // ICW2: Master pic vector offset
    IO::Wait();
    IO::OutB(PIC2_DATA, offset2); // ICW2: Slave pic vector offset
    IO::Wait();

    IO::OutB(PIC1_DATA, 4); // ICW3 : Tell Master pic that there is a slave pic at IRQ2 (0000 0100)
    IO::Wait();
    IO::OutB(PIC2_DATA, 2); // ICW3: Tell Slave pic its cascade identity (0000 0010)
    IO::Wait();

    IO::OutB(PIC1_DATA, ICW4_8086); // ICW4: have the pics use 8086 mode (and not 8080 mode)
    IO::Wait();
    IO::OutB(PIC2_DATA, ICW4_8086);
    IO::Wait();

    // Unmask both pics
    IO::OutB(PIC1_DATA, 0);
    IO::OutB(PIC2_DATA, 0);

    log(INFO, "PIC - Initialized!\n");
}

void PIC::Mask(uint8_t irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }
    
    value = IO::InB(port) | (1 << irq);
    IO::OutB(port, value);
}

void PIC::Unmask(uint8_t irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else 
    {
        port = PIC2_DATA;
        irq -= 8;
    }
    
    value = IO::InB(port) & ~(1 << irq);
    IO::OutB(port, value);
}

void PIC::Disable()
{
    // Mask all the interrupts
    IO::OutB(PIC1_DATA, 0xff);
    IO::OutB(PIC2_DATA, 0xff);
}

void PIC::SendEOI(uint8_t irq)
{
    if (irq >= 8)
    {
        IO::OutB(PIC2_COMMAND, PIC_EOI);
    }

    IO::OutB(PIC1_COMMAND, PIC_EOI);
}

// read the cascaded value of request reg
uint16_t PIC::GetIRR()
{
    IO::OutB(PIC1_COMMAND, PIC_READ_IRR);
    IO::OutB(PIC2_COMMAND, PIC_READ_IRR);
    return (IO::InB(PIC2_COMMAND) << 8) | IO::InB(PIC1_COMMAND);
}

// read the cascaded value of service reg
uint16_t PIC::GetISR()
{
    IO::OutB(PIC1_COMMAND, PIC_READ_ISR);
    IO::OutB(PIC2_COMMAND, PIC_READ_ISR);
    return (IO::InB(PIC2_COMMAND) << 8) | IO::InB(PIC1_COMMAND);
}
