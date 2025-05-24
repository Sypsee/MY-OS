#include <stdint.h>
#include "core/irq.h"

struct KeyInfo {
    uint8_t scancode = 0;
    char currentChar = 0;
    bool shift = false;
    bool caps = false;
    bool ctrl = false;
};

class Keyboard {
public:
    static void Init();
    static void InterruptHandler(StackFrame *frame);
    static KeyInfo &getKeyInfo();

private:
    static KeyInfo keyInfo;
};
