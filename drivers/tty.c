/*
 * Copyright (C) 2025 Roy Roy123ty@hotmail.com
 * 
 * This file is part of Solum OS
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "solum.h"
#include "boot/li_req.h"
#ifdef __x86_64__
#include "drivers/video/screen.h"
#endif
#include "drivers/port/serial.h"
#include "kernel/msg/logk.h"
#include "kernel/msg/printk.h"
#include "libc/string.h"
#include "arch.h"
#include "atomic.h"
#include "drivers/tty.h"

#define CONSOLE_DEVICE_MAX_LEN 16
#define CMDLINE_CONSOLE_PREFIX "console="
#define CMDLINE_CONSOLE_PREFIX_LEN 8

static volatile int tty_lock = 0;
static bool out_screen = false;
static bool out_serial[MAX_SERIAL_DEVS] = {0};
static const uintptr_t serial_ports[MAX_SERIAL_DEVS] = {
    SERIAL_COM1, SERIAL_COM2, SERIAL_COM3, SERIAL_COM4
};

static struct {
    char data[TTY_BUFFER_SIZE];
    size_t head;                   
    size_t tail;                 
    bool flush_on_newline;
    bool buffer_full;             
} tty_ring_buffer = {.head = 0, .tail = 0, .flush_on_newline = true, .buffer_full = false};

static inline void tty_lock_acquire();
static inline void tty_lock_release();
#ifdef __x86_64__
static void tty_output_char_screen(char c);
#endif
static void tty_parse_cmdline(const char *cmdline);
static void tty_init_devices();
static void tty_flush_buffer();
static void tty_buffer_putc(char c);
static void tty_buffer_puts(const char *s);
static int tty_device_amount ();

static inline void tty_lock_acquire() {
    while (atomic_exchange_int(&tty_lock, 1)) {
        pause();
    }
}

static inline void tty_lock_release() {
    atomic_store_release_int(&tty_lock, 0);
}

#ifdef __x86_64__
static void tty_output_char_screen(char c) {
    if (c == '\n') {
        cursor_x = SCREEN_MARGIN;
        print_newline();
        return;
    }

    if (c < 32 || c >= 127) {
        screen_putc('?', cursor_x, cursor_y, VIDEO_WHITE);
        cursor_x += CHAR_W;
        return;
    }
    
    screen_putc(c, cursor_x, cursor_y, VIDEO_WHITE);
    cursor_x += CHAR_W;
    
    if (cursor_x > screen_width - SCREEN_MARGIN - CHAR_W) {
        cursor_x = SCREEN_MARGIN;
        print_newline();
    }
}
#endif

static void tty_buffer_putc(char c) {
    if (tty_ring_buffer.buffer_full) {
        tty_flush_buffer(); 
    }
    
    tty_ring_buffer.data[tty_ring_buffer.head] = c;
    tty_ring_buffer.head = (tty_ring_buffer.head + 1) % TTY_BUFFER_SIZE;
    
    tty_ring_buffer.buffer_full = (tty_ring_buffer.head == tty_ring_buffer.tail);
    
    if (tty_ring_buffer.flush_on_newline && c == '\n') {
        tty_flush_buffer();
    }
}

static void tty_buffer_puts(const char *s) {
    if (!s) return;
    
    while (*s) {
        tty_buffer_putc(*s);
        s++;
    }
}

static void tty_flush_buffer() {
    if (!tty_ring_buffer.buffer_full && tty_ring_buffer.head == tty_ring_buffer.tail) {
        return;
    }
    
    size_t chars_to_flush;
    if (tty_ring_buffer.buffer_full) {
        chars_to_flush = TTY_BUFFER_SIZE;
    } else if (tty_ring_buffer.head >= tty_ring_buffer.tail) {
        chars_to_flush = tty_ring_buffer.head - tty_ring_buffer.tail;
    } else {
        chars_to_flush = TTY_BUFFER_SIZE - tty_ring_buffer.tail + tty_ring_buffer.head;
    }
    
    for (size_t i = 0; i < chars_to_flush; i++) {
        char c = tty_ring_buffer.data[tty_ring_buffer.tail];
        
        for (int s = 0; s < MAX_SERIAL_DEVS; s++) {
            if (out_serial[s]) {
                serial_putc(c, serial_ports[s]);
            }
        }

#ifdef __x86_64__
        if (out_screen && c) {
            tty_output_char_screen(c);
        }
#endif
        
        tty_ring_buffer.tail = (tty_ring_buffer.tail + 1) % TTY_BUFFER_SIZE;
    }
    
    tty_ring_buffer.buffer_full = false;
}

static void tty_parse_cmdline(const char *cmdline) {
    if (!cmdline) return;
    
    size_t len = strlen(cmdline);
    const char *ptr = cmdline;
 
    while (ptr < cmdline + len) {
        const char *console_pos = strstr(ptr, CMDLINE_CONSOLE_PREFIX);
        if (!console_pos) break;
        
        ptr = console_pos + CMDLINE_CONSOLE_PREFIX_LEN;

        const char *param_end = ptr;
        while (*param_end && *param_end != ' ') param_end++;

        const char *device_start = ptr;
        while (device_start < param_end) {
            const char *comma = device_start;
            while (comma < param_end && *comma != ',') comma++;
            
            size_t device_len = comma - device_start;
            if (device_len > 0 && device_len < CONSOLE_DEVICE_MAX_LEN) {
                char device[CONSOLE_DEVICE_MAX_LEN];
                memcpy(device, device_start, device_len);
                device[device_len] = '\0';
                
                if (strcmp(device, "tty0") == 0) {
                    out_screen = true;
                } else if (strncmp(device, "ttyS", 4) == 0) {
                    const char *num_str = device + 4;
                    int port_num = 0;
                    
                    while (*num_str >= '0' && *num_str <= '9') {
                        port_num = port_num * 10 + (*num_str - '0');
                        num_str++;
                    }
                    
                    if (port_num >= 0 && port_num < MAX_SERIAL_DEVS) {
                        out_serial[port_num] = true;
                    }
                }
            }
            
            device_start = comma + (*comma == ',' ? 1 : 0);
        }
        
        ptr = param_end;
    }

    
#ifdef __x86_64__
bool any = out_screen;
for (int i = 0; i < MAX_SERIAL_DEVS; i++) {
    if (out_serial[i]) {
        any = true;
        break;
    }
}
if (!any) {
    out_screen = true;
    out_serial[0] = true;
}
#else
bool any = false;
for (int i = 0; i < MAX_SERIAL_DEVS; i++) {
    if (out_serial[i]) {
        any = true;
        break;
    }
}
if (!any) {
    out_serial[0] = true;
}
#endif
    }

static void tty_init_devices() {
#ifdef __x86_64__
    if (out_screen) {
        screen_init();
    }
#endif
    
    for (int i = 0; i < MAX_SERIAL_DEVS; i++) {
        if (out_serial[i]) {
            serial_init(serial_ports[i]);
            serial_putc('\n', serial_ports[i]);
        }
    }
}

static int tty_device_amount () {
    unsigned int j = 0;
    for (int i = 0; i < MAX_SERIAL_DEVS; i++) {
        if (out_serial[i]) {
            j++;
        }
    }

    if (out_screen) {
        return (unsigned) (1 + j);
    } else if (!out_screen) {
        return (unsigned) j;
    } else {
        halt();
    }
}

void tty_init() {
    const char *cmdline = get_cmdline();

    tty_parse_cmdline(cmdline);

    tty_init_devices();

    logk(KERN_INFO"TTY: Device initialized\n");
    logk(KERN_INFO"TTY: %d Devices enabled\n", tty_device_amount());

}

int tty_putc(char c) {
    tty_lock_acquire();
    
    tty_buffer_putc(c);
    
    tty_lock_release();
    return 1;
}

int tty_write(const char *buf, size_t len) {
    if (!buf || len == 0) return 0;
    
    tty_lock_acquire();
    
    for (size_t i = 0; i < len; i++) {
        tty_buffer_putc(buf[i]);
    }
    
    tty_lock_release();
    return (int)len;
}

int tty_puts(const char *s) {
    if (!s) return 0;
    
    tty_lock_acquire();
    
    tty_buffer_puts(s);
    
    tty_lock_release();
    return strlen(s);
}

void tty_flush() {
    tty_lock_acquire();
    tty_flush_buffer();
    tty_lock_release();
}

size_t tty_buffer_usage() {
    tty_lock_acquire();
    size_t usage;
    
    if (tty_ring_buffer.buffer_full) {
        usage = TTY_BUFFER_SIZE;
    } else if (tty_ring_buffer.head >= tty_ring_buffer.tail) {
        usage = tty_ring_buffer.head - tty_ring_buffer.tail;
    } else {
        usage = TTY_BUFFER_SIZE - tty_ring_buffer.tail + tty_ring_buffer.head;
    }
    
    tty_lock_release();
    return usage;
}