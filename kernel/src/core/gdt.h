#pragma once

#include <stdint.h>

namespace GDT {
struct [[gnu::packed]] Entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags_limit_high;
    uint8_t base_high;

    static constexpr Entry Create(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
        return Entry{
            static_cast<uint16_t>(limit & 0xFFFF),
            static_cast<uint16_t>(base & 0xFFFF),
            static_cast<uint8_t>((base >> 16) & 0xFF),
            access,
            static_cast<uint8_t>(((limit >> 16) & 0x0F) | (flags & 0xF0)),
            static_cast<uint8_t>((base >> 24) & 0xFF)
        };
    }
};

struct [[gnu::packed]] Descriptor {
    uint16_t limit;
    Entry* ptr;
};

enum Access : uint8_t {
    CodeReadable = 0x02,
    DataWritable = 0x02,
    CodeConforming = 0x04,
    DataDirectionNormal = 0x00,
    DataDirectionDown = 0x04,
    DataSegment = 0x10,
    CodeSegment = 0x18,
    DescriptorTSS = 0x00,
    Ring0 = 0x00,
    Ring1 = 0x20,
    Ring2 = 0x40,
    Ring3 = 0x60,
    Present = 0x80
};

enum Flags : uint8_t {
    Bit64 = 0x20,
    Bit32 = 0x40,
    Bit16 = 0x00,
    Granularity1B = 0x00,
    Granularity4K = 0x80
};

class Manager {
public:
    static void Init();
private:
    static Entry gdt[];
    static Descriptor descriptor;
};
}