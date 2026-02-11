#include "solum.h"
#include "drivers/tty.h"
#include "drivers/keyboard.h"
#include "libc/string.h"
#include "arch.h"
#include "kernel/msg/printk.h"

#ifdef __x86_64__
#include "drivers/video/screen.h"
#include "drivers/port/port.h"
#endif

#define SHELL_BUFFER_SIZE 256
#define SHELL_PROMPT "solum> "

static void shell_cmd_help() {
    printk("Available commands:\n");
    printk("  help    - Show this help message\n");
    printk("  clear   - Clear the screen\n");
    printk("  echo    - Print text to screen\n");
    printk("  version - Show OS version\n");
    printk("  reboot  - Reboot the system\n");
    printk("  halt    - Halt the system\n");
}

static void shell_cmd_clear() {
#ifdef __x86_64__
    screen_clear(VIDEO_BLACK);
#endif
}

static void shell_cmd_echo(char *args) {
    if (args && strlen(args) > 0) {
        printk("%s\n", args);
    }
}

static void shell_cmd_version() {
    printk("Solum OS "VER_FUL"\n");
}

// 这里不知道为什么会导致Pause而不是重启
// 以后再修
// TODO
static void shell_cmd_reboot() {
    printk("Rebooting...\n");
    asm volatile ("cli");
    
    uint64_t idt_ptr[2];
    idt_ptr[0] = 0;
    idt_ptr[1] = 0;
    
    asm volatile ("lidt %0" : : "m" (idt_ptr));
    
    asm volatile ("int $0");
}

static void shell_cmd_halt() {
    printk("Halting...\n");
    halt();
}

static void shell_execute(char *cmd) {
    char *args = NULL;
    char *space = strchr(cmd, ' ');
    
    if (space) {
        *space = '\0';
        args = space + 1;
    }

    if (strcmp(cmd, "help") == 0) {
        shell_cmd_help();
    } else if (strcmp(cmd, "clear") == 0) {
        shell_cmd_clear();
    } else if (strcmp(cmd, "echo") == 0) {
        shell_cmd_echo(args);
    } else if (strcmp(cmd, "version") == 0) {
        shell_cmd_version();
    } else if (strcmp(cmd, "reboot") == 0) {
        shell_cmd_reboot();
    } else if (strcmp(cmd, "halt") == 0) {
        shell_cmd_halt();
    } else if (strlen(cmd) > 0) {
        printk("Unknown command: %s\n", cmd);
        printk("Type 'help' for available commands\n");
    }
}

void shell_run() {
    char buffer[SHELL_BUFFER_SIZE];
    int pos = 0;

    printk("\n");
    printk("Solum OS Shell\n");
    printk("Type 'help' for available commands\n");
    printk("\n");

    while (1) {
        printk(SHELL_PROMPT);
        tty_flush();

        pos = 0;
        while (1) {
            char c = keyboard_getc();

            if (c == '\n') {
                buffer[pos] = '\0';
                printk("\n");
                tty_flush();

                if (pos > 0) {
                    shell_execute(buffer);
                }

                break;
            } else if (c == '\b') {
                if (pos > 0) {
                    pos--;
#ifdef __x86_64__
                    cursor_x -= CHAR_W;
                    if (cursor_x < SCREEN_MARGIN) {
                        cursor_x = SCREEN_MARGIN;
                    }
                    screen_clear_char(cursor_x, cursor_y, 0x000000);
#else
                    tty_putc(' ');
#endif
                    tty_flush();
                }
            } else if (c >= 32 && c < 127 && pos < SHELL_BUFFER_SIZE - 1) {
                buffer[pos++] = c;
                printk("%c", c);
                tty_flush();
            }
        }
    }
}
