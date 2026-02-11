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

CC := x86_64-linux-gnu-gcc
LD := x86_64-linux-gnu-ld
AS := x86_64-linux-gnu-as
KELF = build/$(ARCH)/slkrnl_x64.elf
ISO = Solum_x86_64.iso

ifeq ($(BUILD),release)
CFLAGS := -c -O3 -I$(INCDIR1) -I$(INCDIR2) -nostdlib -nostartfiles -nodefaultlibs -mno-red-zone -ffreestanding -z noexecstack
else ifeq ($(BUILD),debug)
CFLAGS := -g -c -O0 -I$(INCDIR1) -I$(INCDIR2) -nostdlib -nostartfiles -nodefaultlibs -mno-red-zone -ffreestanding -z noexecstack
endif

run_b: info $(ISO)
	@qemu-system-x86_64 -m 1G -cdrom $(ISO) -no-shutdown -no-reboot

run: info $(ISO)
	@qemu-system-x86_64 -m 1G -bios assets/X86_64_EFI.fd -cdrom $(ISO) -no-shutdown -no-reboot