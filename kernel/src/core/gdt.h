#pragma once

#include <stdint.h>

struct __attribute__((packed)) GDT_Entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags_limit_high;
    uint8_t base_high;
};

struct __attribute__((packed)) GDT_Descriptor
{
    uint16_t limit;
    GDT_Entry* ptr;
};

enum GDT_ACCESS
{
    CODE_READABLE = 0x02,
    DATA_WRITABLE = 0x02,

    CODE_CONFORMING = 0x04,
    DATA_DIRECTION_NORMAL = 0x00,
    DATA_DIRECTION_DOWN = 0x04,

    DATA_SEGMENT = 0x10,
    CODE_SEGMENT = 0x18,

    DESCRIPTOR_TSS = 0x00,

    GDT_FLAG_RING0 = 0x00,
    GDT_FLAG_RING1 = 0x20,
    GDT_FLAG_RING2 = 0x40,
    GDT_FLAG_RING3 = 0x60,

    GDT_FLAG_PRESENT = 0x80,
};

enum GDT_FLAGS
{
    BIT_64 = 0x20,
    BIT_32 = 0x40,
    BIT_16 = 0x00,

    GRANULARITY_1B = 0x00,
    GRANULARITY_4K = 0x80,
};

#define GDT_LIMIT_LOW(limit) (limit & 0xFFFF)
#define GDT_BASE_LOW(base) (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base) ((base >> 16) & 0xFF)
#define GDT_FLAGS_LIMIT_HIGH(limit, flags) (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base) ((base >> 24) & 0xFF)

#define GDT_ENTRY(base, limit, access, flags) { \
    GDT_LIMIT_LOW(limit), \
    GDT_BASE_LOW(base), \
    GDT_BASE_MIDDLE(base), \
    access, \
    GDT_FLAGS_LIMIT_HIGH(limit, flags), \
    GDT_BASE_HIGH(base), \
}

class GDT
{
public:
    static void Init();
};