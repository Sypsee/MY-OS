#include "RTC.h"

#include "core/io.h"
#include <utils/logger.h>

DateTime RTC::dateTime = {};

// helper func
uint8_t GetRTCRegister(uint8_t reg) {
    IO::OutB(0x70, reg);
    return IO::InB(0x71);
}

void RTC::Init() {
	uint8_t prev_conf = GetRTCRegister(0xB);
	IO::OutB(0x70, 0xB);
	IO::OutB(0x71, prev_conf | 0x10); // enable UEI(Update Ended Interrupt) mode

    // for some reason its not cleared?
    GetRTCRegister(0x0C);
}

void RTC::InterruptHandler(StackFrame *frame) {
    dateTime.second = GetRTCRegister(0x0);
    dateTime.minute = GetRTCRegister(0x2);
    dateTime.hour = GetRTCRegister(0x4);
    dateTime.day = GetRTCRegister(0x7);
    dateTime.month = GetRTCRegister(0x8);
    dateTime.year = GetRTCRegister(0x9);

    // Convert BCD to Binary
    if (!(GetRTCRegister(0x0B) & 0x04)) {
        dateTime.second = (dateTime.second & 0x0F) + ((dateTime.second / 16) * 10);
        dateTime.minute = (dateTime.minute & 0x0F) + ((dateTime.minute / 16) * 10);
        dateTime.hour = ( (dateTime.hour & 0x0F) + (((dateTime.hour & 0x70) / 16) * 10) ) | (dateTime.hour & 0x80);
        dateTime.day = (dateTime.day & 0x0F) + ((dateTime.day / 16) * 10);
        dateTime.month = (dateTime.month & 0x0F) + ((dateTime.month / 16) * 10);
        dateTime.year = (dateTime.year & 0x0F) + ((dateTime.year / 16) * 10);
    }

	// Clear register C to continue updates
    GetRTCRegister(0x0C);
}

uint8_t RTC::getSeconds() {
    return dateTime.second;
}

uint8_t RTC::getMinutes() {
    return dateTime.minute;
}

uint8_t RTC::getHours() {
    return dateTime.hour;
}

uint8_t RTC::getDay() {
    return dateTime.day;
}

uint8_t RTC::getMonth() {
    return dateTime.month;
}

uint8_t RTC::getYear() {
    return dateTime.year;
}

DateTime RTC::getDateTime() {
    return dateTime;
}