English Docs | [中文文档](readme/README_zh.md)
   
<div align="center"><img title="" src="readme/icon.jpeg" width="35%"></div>

## <div align="center"> Solum OS </div>

>*A General,Stable,Compatible,Next Generation Solum Operating System*

## Project Overview

Solum OS is an x86_64 aarch64 longarch64 RISCV64 operating system project based on Limine, currently in its early development stage.

## Build Instructions

### Dependencies
- git
- make
- xorriso
- GCC Toolchain (gcc-x86-64-linux-gnu gcc-aarch64-linux-gnu ...)
- QEMU (qemu-system-x86_64 qemu-system-aarch64 ...)

### Build Commands

```bash
make help                  # Show Help
make prep                  # Run it before build
make iso                   # Build ISO (default x86_64)
make iso ARCH=aarch64      # Build ISO (aarch64)
# ARCH parameter is allowed (placed at the end)
make run_b                 # Build and boot QEMU using BIOS (only x86_64)
make run                   # Build and boot QEMU using UEFI
make clean                 # Clean build files
```

## Contributing
Welcome to submit Issues and Pull Requests to help improve Solum OS!

## License
This project uses GPL 3.0 License. And used some source from some other open source projects.

## Open source projects referenced or used

- Uinxed-Kernel: [https://github.com/ViudiraTech/Uinxed-Kernel](https://github.com/ViudiraTech/Uinxed-Kernel) (Apache 2.0)
- CoolPotOS: [https://github.com/plos-clan/CoolPotOS](https://github.com/plos-clan/CoolPotOS) (MIT License)
- NeoAetherOS: [https://github.com/aether-os-studio/naos](https://github.com/aether-os-studio/naos) (GPL 3.0)
  
>*Solum OS - Building the Next Generation General Purpose Operating System*
