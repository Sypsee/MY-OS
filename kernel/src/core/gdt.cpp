#include "gdt.h"
#include <utils/logger.h>

GDT_Entry g_GDT[] =
{
    // NULL DESCRIPTOR
    GDT_ENTRY(0, 0, 0, 0),

    // CODE SEG
    GDT_ENTRY
    (
        0,
        0xFFFFF,
        PRESENT | RING0 | CODE_SEGMENT | CODE_READABLE,
        BIT_64 | GRANULARITY_4K
    ),

    // DATA SEG
    GDT_ENTRY
    (
        0,
        0xFFFFF,
        PRESENT | RING0 | DATA_SEGMENT | DATA_WRITABLE,
        BIT_64 | GRANULARITY_4K
    ),
};

GDT_Descriptor g_GDT_Descriptor = {
    sizeof(g_GDT) - 1,
    g_GDT
};

void setup_gdt()
{
    __asm__ volatile("cli");
    __asm__ volatile("lgdt %0" :: "m"(g_GDT_Descriptor): "memory");
    __asm__ volatile(
        "pushq $0x08\n"
        "lea 1f(%%rip), %%rax\n"
        "pushq %%rax\n"
        "lretq\n"
        "1:\n"
        :
        :
        : "rax", "memory"
    );

    __asm__ volatile(
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :
        :
        : "memory"
    );

    __asm__ volatile("sti");

    log(INFO, "GDT INITIALIZED!\n");
}