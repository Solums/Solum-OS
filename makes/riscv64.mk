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

CC := riscv64-linux-gnu-gcc
LD := riscv64-linux-gnu-ld
AS := riscv64-linux-gnu-as
KELF = build/$(ARCH)/slkrnl_RC64.elf
ISO = Solum_riscv64.iso

ifeq ($(BUILD),release)
CFLAGS := -c -O3 -I$(INCDIR1) -I$(INCDIR2) -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -z noexecstack
else ifeq ($(BUILD),debug)
CFLAGS := -g -c -O0 -I$(INCDIR1) -I$(INCDIR2) -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -z noexecstack
endif

run: info $(ISO)
	@cp assets/RISCV_VIRT_VARS.fd.back assets/RISCV_VIRT_VARS.fd
	@qemu-system-riscv64 \
  		-nographic \
  		-machine virt,pflash0=pflash0,pflash1=pflash1 \
  		-m 1G \
  		-blockdev node-name=pflash0,driver=file,read-only=on,filename=assets/RISCV_VIRT_CODE.fd \
  		-blockdev node-name=pflash1,driver=file,filename=assets/RISCV_VIRT_VARS.fd \
  		-drive file=$(ISO),format=raw,id=cd,media=cdrom \
  		-device virtio-blk-device,drive=cd \