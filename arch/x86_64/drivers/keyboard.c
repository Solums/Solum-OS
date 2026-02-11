#include "solum.h"
#include "drivers/port/port.h"
#include "drivers/keyboard.h"
#include <stdbool.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEYBOARD_STATUS_OBF 0x01

static char scancode_to_char(uint8_t scancode, bool shift_pressed) {
    static const char scancode_table[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' '
    };

    static const char scancode_table_shift[] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
        0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
        '*', 0, ' '
    };

    if (scancode >= sizeof(scancode_table)) {
        return 0;
    }

    if (shift_pressed) {
        return scancode_table_shift[scancode];
    } else {
        return scancode_table[scancode];
    }
}

void keyboard_init() {
    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OBF) {
        inb(KEYBOARD_DATA_PORT);
    }
}

char keyboard_getc() {
    static bool shift_pressed = false;

    while (true) {
        uint8_t status = inb(KEYBOARD_STATUS_PORT);

        if (status & KEYBOARD_STATUS_OBF) {
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);

            if (scancode & 0x80) {
                if (scancode == 0xAA || scancode == 0xB6) {
                    shift_pressed = false;
                }
            } else {
                if (scancode == 0x2A || scancode == 0x36) {
                    shift_pressed = true;
                } else {
                    char c = scancode_to_char(scancode, shift_pressed);
                    if (c) {
                        return c;
                    }
                }
            }
        }
    }
}
