#include "Keyboard.h"
#include "core/io.h"
#include "utils/logger.h"

#define KEY_ENC_CMD_PORT 0x60
#define KEY_CTRL_CMD_PORT 0x64

KeyInfo Keyboard::keyInfo = {};

char asciiMap[255] = {
    '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 
    '\0', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
    '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', ' ', '*', 
    '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', 
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', 
    '\0'
};

uint8_t ReadKeyEnc() {
    //while (!(IO::InB(KEY_CTRL_CMD_PORT) & 1));
    return IO::InB(KEY_ENC_CMD_PORT);
}

void WriteKeyEnc(uint8_t data) {
    while ((IO::InB(KEY_CTRL_CMD_PORT) & 2));
    IO::OutB(KEY_ENC_CMD_PORT, data);
}

void WriteKeyCTRL(uint8_t data) {
    while ((IO::InB(KEY_CTRL_CMD_PORT) & 2));
    IO::OutB(KEY_CTRL_CMD_PORT, data);
}

void Keyboard::Init() {
    WriteKeyCTRL(0xAE);
    WriteKeyEnc(0xF4);
    if (ReadKeyEnc() != 0xFA) log(WARN, "Could not enable scan!\n");
}

char ChangeSpecialCharAsShift(char currentChar) {
    switch (currentChar)
    {
        case '1' : currentChar = '!'; break;
        case '2' : currentChar = '@'; break;
        case '3' : currentChar = '#'; break;
        case '4' : currentChar = '$'; break;
        case '5' : currentChar = '%'; break;
        case '6' : currentChar = '^'; break;
        case '7' : currentChar = '&'; break;
        case '8' : currentChar = '*'; break;
        case '9' : currentChar = '('; break;
        case '0' : currentChar = ')'; break;

        case '-' : currentChar = '_'; break;
        case '=' : currentChar = '+'; break;
        case ';' : currentChar = ':'; break;
        case '.' : currentChar = '>'; break;
        case ',' : currentChar = '<'; break;
        case '/' : currentChar = '?'; break;
        case '\\' : currentChar = '|'; break;
        case '[' : currentChar = '{'; break;
        case ']' : currentChar = '}'; break;
        case '`' : currentChar = '~'; break;
        case '\'' : currentChar = '\"'; break;

        default: break;
    }

    return currentChar;
}

bool isalpha(char c, bool uppercase) {
    return (uppercase && c >= 'A' && c <= 'Z') || (!uppercase && c >= 'a' && c <= 'z');
}

void Keyboard::InterruptHandler(StackFrame *frame) {
    static bool breakCode = false;

    keyInfo.scancode = ReadKeyEnc();
    if (keyInfo.scancode == 0xFA) {
        keyInfo.scancode = 0;
        return;
    }

    if (keyInfo.scancode == 0x2A || keyInfo.scancode == 0x36) {
        keyInfo.shift = true;
        return;
    }
    else if (keyInfo.scancode == 0xB6 || keyInfo.scancode == 0xAA) {
        keyInfo.shift = false;
        return;
    }

    if (keyInfo.scancode == 0x3A) {
        keyInfo.caps = !keyInfo.caps;
        return;
    }

    if (keyInfo.scancode == 0x1D) {
        keyInfo.ctrl = true;
        return;
    }
    else if (keyInfo.scancode == 0x9D) {
        keyInfo.ctrl = false;
        return;
    }

    char c = asciiMap[keyInfo.scancode];
    if (isalpha(c, true) || isalpha(c, false)) {
        if (keyInfo.shift ^ keyInfo.caps) {
            if (isalpha(c, false)) c -= 0x20;
        } else {
            if (isalpha(c, true)) c += 0x20;
        }
    }
    else if (keyInfo.shift) {
        c = ChangeSpecialCharAsShift(c);
    }

    keyInfo.currentChar = c;
    printf("%c", c);
}


KeyInfo &Keyboard::getKeyInfo() {
    return keyInfo;
}
