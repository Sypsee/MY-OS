#pragma once

#include <stdint.h>

class IO
{
public:
    static bool Init();
    static void OutB(uint16_t port, uint8_t value);
    static uint8_t InB(uint16_t port);
    static void OutW(uint16_t port, uint16_t value);
    static uint16_t InW(uint16_t port);
    static void OutL(uint16_t port, uint32_t value);
    static uint32_t InL(uint16_t port);
    static void Wait();
};