#
# Copyright (C) 2025 Roy Roy123ty@hotmail.com
# 
# This file is part of Solum OS
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

BUILD ?= release
ARCH ?= x86_64

INCDIR1 := include 
INCDIR2 := arch/$(ARCH)/include

SRC_DIRS := arch/$(ARCH) boot drivers init libs kernel
C_FILE := $(shell find $(SRC_DIRS) -name "*.c" -type f)
S_FILE := $(shell find $(SRC_DIRS) -name "*.as" -type f)

C_O_FILE := $(patsubst %.c, build/$(ARCH)/%.o, $(C_FILE))
S_O_FILE := $(patsubst %.as, build/$(ARCH)/%.o, $(S_FILE))
O_FILE := $(S_O_FILE) $(C_O_FILE)

A = @printf "\033[1;32m[Build]\033[0m $@ ...\n";

BUILD_YEAR = $(shell date +%y)
BUILD_DATE = $(shell date +%Y%m%d)
BUILD_TIME = $(shell date +%H%M)

all: info iso

ifeq ($(ARCH),x86_64)
include makes/x86_64.mk
else ifeq ($(ARCH),aarch64)
include makes/aarch64.mk
else ifeq ($(ARCH),riscv64)
include makes/riscv64.mk
endif

iso: $(ISO)

info:
	@printf "\n"
	@printf "Solum OS Build Script.\n"
	@printf "Copyright 2025 Roy, based on the GPL 3.0 license.\n"
	@printf "Thanks for Downloading the source.\n"
	@printf "Also thanks for github.com/limine-bootloader/limine github.com/ViudiraTech/Uinxed-Kernel\n"
	@printf "github.com/aether-os-studio/naos github.com/plos-clan/CoolPotOS\n"
	@printf "                Giving them a STAR on github :) !\n"
	@echo

$(ISO): $(KELF) arch/$(ARCH)/limine.conf
	$(A)printf "\033[1;32m[ISO]\033[0m Packing ISO file...\n"
	@cd build/isodir/Solum && if [ -e "slkrnl_AA64.elf" ] || [ -e "slkrnl_RC64.elf" ] || [ -e "slkrnl_x64.elf" ]; then rm *.elf; fi
	@cp $(KELF) build/isodir/Solum
	@cp arch/$(ARCH)/limine.conf build/isodir/limine
	@xorriso -as mkisofs -R -r -J -b \
	limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
	-hfsplus -apm-block-size 2048 --efi-boot limine/limine-uefi-cd.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label --volid "Solum OS" build/isodir -o $(ISO)

$(KELF): $(O_FILE)
	$(A)$(LD) -n -T arch/$(ARCH)/linker.ld -o $(KELF) $(O_FILE) -z noexecstack

build/$(ARCH)/%.o: %.c
	@mkdir -p $(dir $@)
	$(A)$(CC) $(CFLAGS) -DBUILD_DATE=\"$(BUILD_DATE)\" -DBUILD_TIME=\"$(BUILD_TIME)\" -DBUILD_YEAR=\"$(BUILD_YEAR)\" $< -o $@

build/$(ARCH)/%.o: %.as
	@mkdir -p $(dir $@)
	$(A)$(AS) -c $< -o $@

clean:
	@rm -f $(KELF) $(ISO)
	@rm -rf build/$(ARCH)/*
	@rm -f *.elf
	@printf "\033[1;32m[Done]\033[0m Clean completed.\n"

help:
	@printf "Solum OS Makefile Usage:\n"
	@printf "  make prep 				- Do the preperation automatically.\n"
	@printf "  make iso         		- Build the entire project.\n"
	@printf "           ARCH=x86_64/aarch64/riscv64  really?\n"
	@printf "  make run_b/run_u   		- Run the in QEMU on BIOS/UEFI.\n"
	@printf "             	   ARCH=x86_64/aarch64/riscv64\n"
	@printf "  make clean       		- Clean all generated files.\n"
	@printf "  make help        		- Display this help message.\n\n"

prep:

# create isodir
	@if [ ! -d "build/isodir/EFI/BOOT" ]; then mkdir -p build/isodir/EFI/BOOT; fi
	@if [ ! -d "build/isodir/limine" ]; then mkdir build/isodir/limine; fi
	@if [ ! -d "build/isodir/Solum" ]; then mkdir build/isodir/Solum; fi

# create bootloader
	@if [ ! -d "assets/Limine" ]; then cd assets && printf "\n" && printf "I'm Working hard. Don't press your Ctrl and C. They are dirty and smells awful !!\n" && \
	git clone https://codeberg.org/Limine/Limine.git --branch=v10.5.0-binary --depth=1 && rm Limine/.gitignore && rm -rf Limine/.git \
	&& rm Limine/limine-bios-hdd.h && rm Limine/limine.c && rm Limine/Makefile; fi
	@if [ ! -e "build/isodir/EFI/BOOT/BOOTAA64.EFI" ] || [ ! -e "build/isodir/EFI/BOOT/BOOTLOONGARCH64.EFI" ] || [ ! -e "build/isodir/EFI/BOOT/BOOTRISCV64.EFI" ] || [ ! -e "build/isodir/EFI/BOOT/BOOTX64.EFI" ]; then \
		cd assets/Limine && cp BOOTX64.EFI BOOTAA64.EFI BOOTLOONGARCH64.EFI BOOTRISCV64.EFI ../../build/isodir/EFI/BOOT; fi
	@if [ ! -e "build/isodir/limine/limine-uefi-cd.bin" ] || [ ! -e "build/isodir/limine/limine-bios-cd.bin" ] || [ ! -e "build/isodir/limine/limine-bios.sys" ]; then \
		cd assets/Limine && cp limine-uefi-cd.bin limine-bios.sys limine-bios-cd.bin ../../build/isodir/limine; fi

# create build directory 
	@if [ ! -d "build/x86_64" ]; then mkdir -p build/x86_64; fi
	@if [ ! -d "build/aarch64" ]; then mkdir -p build/aarch64; fi
	@if [ ! -d "build/riscv64" ]; then mkdir -p build/riscv64; fi

	@printf "\033[1;32m[OK]\033[0m Preperation work\n"

.PHONY: iso info clean prep 