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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "boot/limine.h"
#include "solum.h"
#include "boot/li_req.h"
#include "libc/stdlib.h"
#include "libc/string.h"

#ifdef __x86_64__
#include "drivers/video/screen.h"
#endif

#include "arch.h"
#include "drivers/tty.h"
#include "kernel/msg/printk.h"
#include "kernel/msg/logk.h"
#include "kernel/msg/panic.h"
#include "drivers/port/serial.h"

static void display_title() {
    printk("========================================\n");
    printk("   Solum OS "VER_FUL"\n");
    printk("========================================\n");
    printk("\n");
}

static void display_bootloader_info() {
    if (bootloader_info_request.response) {
        struct limine_bootloader_info_response *info = bootloader_info_request.response;
        
        printk("Bootloader Information:\n");
        
        if (info->name) {
            printk("  Name: %s\n", info->name);
        }
        
        if (info->version) {
            printk("  Name: %s\n", info->version);
        }
        
        printk("\n");
    }
}

static void display_framebuffer_info() {

    struct limine_framebuffer *framebuffer;
    
    if (!framebuffer) return;
    
    printk("Framebuffer Information:\n");
    
    printk("  Resolution: %d x %d\n", framebuffer->width, framebuffer->height);
    
    printk("  BPP: %d\n", framebuffer->bpp);
    
    printk("\n");
}

static void display_memory_info() {
    if (!memmap_request.response) return;
    
    printk("Memory Information:\n");
    
    uint64_t total_memory = 0;
    uint64_t usable_memory = 0;
    
    for (uint64_t i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];
        total_memory += entry->length;
        
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            usable_memory += entry->length;
        }
    }
    
    printk("  Total Memory: %d MB\n", total_memory / (1024 * 1024));
    
    printk("  Usable Memory: %d MB\n", usable_memory / (1024 * 1024));
    
    printk("\n");
}

static void display_kernel_address_info() {
    if (!kernel_address_request.response) return;
    
    printk("Kernel Address Information:\n");
    
    struct limine_kernel_address_response *addr = kernel_address_request.response;
   
    printk("  Physical Base: %X\n", addr->physical_base);
    
    printk("  Virtual Base: %lX\n", addr->virtual_base);

    printk("\n");
}

static void display_acpi_info() {
    if (!rsdp_request.response) return;
    
    printk("ACPI RSDP Information:\n");
    
    struct limine_rsdp_response *rsdp = rsdp_request.response;

    printk("  Address: %X\n", (uint64_t)rsdp->address);
    
    printk("\n");
    
    printk("\n");
}

static void display_module_info() {
    if (!module_request.response) return;
    
    struct limine_module_response *modules = module_request.response;
    
    printk("Module Information:\n");
    
    if (modules->module_count == 0) {
        printk("  No modules loaded\n");
    } else {
        printk("  Count: %d\n", modules->module_count);

        printk("\n");
        
        for (uint64_t i = 0; i < modules->module_count; i++) {
            struct limine_file *file = modules->modules[i];
            
            printk("  Module %d: ", i + 1);
            
            if (file->path) {
                const char *name = file->path;
                const char *last_slash = name;
                while (*name) {
                    if (*name == '/') last_slash = name + 1;
                    name++;
                }
                printk("%s\n", last_slash);
            } else {
                printk("Unnamed module\n");
            }
        }
    }
    
    printk("\n");
}

#ifdef __x86_64__

void display_ASCII_icon() {
    screen_write("        :=+*####*+=:",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*6,VIDEO_WHITE);              
    screen_write("     -*@@@@@@@@@@@@@@#=.",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*7,VIDEO_WHITE);          
    screen_write("   -%@@@@@#++++***=--==-",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*8,VIDEO_WHITE);          
    screen_write("  +@@@#@@@#:",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*9,VIDEO_WHITE);                      
    screen_write(" *@@@+ -%@@@%+    :*#**+-.",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*10,VIDEO_WHITE);        
    screen_write(":@@@@    =@@@-    =#%@@@@@*",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*11,VIDEO_WHITE);       
    screen_write("+@@@@.   +@@:    .   :%@@@@+",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*12,VIDEO_WHITE);      
    screen_write("=@@@@%:   .    .%@*   .@@@@*               Solum OS "VER_USR,SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*13,VIDEO_WHITE);      
    screen_write(" *@@@@@@#=    :@@@=    %@@@-                 Your next Operating System",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*14,VIDEO_WHITE);      
    screen_write("  .-+**#*-    +%@@@#: =@@@*",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*15,VIDEO_WHITE);       
    screen_write("                :#@@@#@@@*",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*16,VIDEO_WHITE);        
    screen_write("    -==--=***+++=*@@@@@%-",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*17,VIDEO_WHITE);         
    screen_write("    .=#@@@@@@@@@@@@@@*-",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*18,VIDEO_WHITE);           
    screen_write("        :=*##%%##*=:.",SCREEN_MARGIN+CHAR_W*43,SCREEN_MARGIN+CHAR_H*19,VIDEO_WHITE);
}

#endif

void kernel_main() {

    tty_init();
    display_title();
    display_bootloader_info();
    display_framebuffer_info();
    display_memory_info();
    display_kernel_address_info();
    display_acpi_info();
    display_module_info(); 

#ifdef __x86_64__   

    display_ASCII_icon(); 

#endif      

    halt();

}