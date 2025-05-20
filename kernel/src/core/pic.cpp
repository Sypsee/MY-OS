#include "pic.h"

#include "io.h"
#include <utils/logger.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */

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
        port = PIC1_DATA;
    else
        port = PIC2_DATA;
        irq -= 8;
    
    value = IO::InB(port) | (1 << irq);
    IO::OutB(port, value);
}

void PIC::Unmask(uint8_t irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
        port = PIC1_DATA;
    else
        port = PIC2_DATA;
        irq -= 8;
    
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
        IO::OutB(PIC2_COMMAND, PIC_EOI);

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
