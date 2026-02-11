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

CC := aarch64-linux-gnu-gcc
LD := aarch64-linux-gnu-ld
AS := aarch64-linux-gnu-as
KELF = build/$(ARCH)/slkrnl_AA64.elf
ISO = Solum_aarch64.iso

ifeq ($(BUILD),release)
CFLAGS := -c -O3 -I$(INCDIR1) -I$(INCDIR2) -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -z noexecstack
else ifeq ($(BUILD),debug)
CFLAGS := -g -c -O0 -I$(INCDIR1) -I$(INCDIR2) -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -z noexecstack
endif

run: info $(ISO)
	@qemu-system-aarch64 -machine virt -cpu cortex-a57 -m 1G \
	-bios assets/AARCH_EFI.fd \
	-cdrom $(ISO) -nographic