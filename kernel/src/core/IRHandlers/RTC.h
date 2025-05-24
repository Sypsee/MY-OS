#include <stdint.h>

#include "core/irq.h"

struct DateTime {
    uint8_t second=0;
    uint8_t minute=0;
    uint8_t hour=0;
    uint8_t day=0;
    uint8_t month=0;
    uint8_t year=0;
};

class RTC {
public:
    static void Init();
    static uint8_t getSeconds();
    static uint8_t getMinutes();
    static uint8_t getHours();
    static uint8_t getDay();
    static uint8_t getMonth();
    static uint8_t getYear();
    static DateTime &getDateTime();

    static void InterruptHandler(StackFrame *frame);

private:
    static DateTime dateTime;
};