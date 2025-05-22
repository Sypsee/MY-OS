#include "gdt.h"
#include <utils/logger.h>

namespace GDT {
Entry Manager::gdt[] =
{
    // NULL DESCRIPTOR
    Entry::Create(0, 0, 0, 0),

    // CODE SEG
    Entry::Create(
        0,
        0xFFFFF,
        Access::Present | Access::Ring0 | Access::CodeSegment | Access::CodeReadable,
        Flags::Bit64 | Flags::Granularity4K
    ),

    // DATA SEG
    Entry::Create(
        0,
        0xFFFFF,
        Access::Present | Access::Ring0 | Access::DataSegment | Access::DataWritable,
        Flags::Bit64 | Flags::Granularity4K
    ),
};

Descriptor Manager::descriptor = {
    sizeof(gdt) - 1,
    gdt
};

void Manager::Init() {
    asm volatile("cli");
    asm volatile("lgdt %0" :: "m"(descriptor): "memory");
    asm volatile(
        "pushq $0x08\n"
        "lea 1f(%%rip), %%rax\n"
        "pushq %%rax\n"
        "lretq\n"
        "1:\n"
        ::: "rax", "memory"
    );

    asm volatile(
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        ::: "memory"
    );

    asm volatile("sti");

    log(INFO, "GDT - Initialized!, Base: %p\n", descriptor.ptr);
}
}