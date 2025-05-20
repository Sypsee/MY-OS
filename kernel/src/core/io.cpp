#include "io.h"

#define PORT 0x3F8

bool IO::Init()
{
   OutB(PORT + 1, 0x00);
   OutB(PORT + 3, 0x80);
   OutB(PORT + 0, 0x03);
   OutB(PORT + 1, 0x00);
   OutB(PORT + 3, 0x03);
   OutB(PORT + 2, 0xC7);
   OutB(PORT + 4, 0x0B);
   OutB(PORT + 4, 0x1E);
   OutB(PORT + 0, 0xAE);

   if(InB(PORT + 0) != 0xAE)
   {
      return false;
   }

   OutB(PORT + 4, 0x0F);
   return true;
}

void IO::OutB(uint16_t port, uint8_t value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

uint8_t IO::InB(uint16_t port)
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void IO::OutW(uint16_t port, uint16_t value)
{
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

uint16_t IO::InW(uint16_t port)
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void IO::OutL(uint16_t port, uint32_t value)
{
    __asm__ volatile("outl %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

uint32_t IO::InL(uint16_t port)
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void IO::Wait()
{
    OutB(0x80, 0); // unused port
}