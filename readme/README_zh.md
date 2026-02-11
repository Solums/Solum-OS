[English Docs](../README.md) | 中文文档  

<div align="center"><img title="" src="icon.jpeg" width="35%"></div>

## <div align="center"> Solum OS </div>

>*通用·稳定·兼容——新世代通用操作系统*

## 项目概述

Solum OS 是基于 Limine 的 x86_64 aarch64 longarch64 RISCV64 操作系统项目，目前处于早期开发阶段。

## 构建指南

### 环境依赖
- git
- make
- xorriso
- GCC 工具链 (gcc-x86-64-linux-gnu gcc-aarch64-linux-gnu ...)
- QEMU       (qemu-system-x86_64 qemu-system-aarch64 ...)

### 构建命令

```bash
make help                   # 显示帮助
make prep                   # 构建前运行
make iso                    # 构建ISO (默认x86_64)
make iso ARCH=aarch64       # 构建ISO (aarch64)
# 皆允许 ARCH 参数(放在最后)
make run_b                  # 构建并使用BIOS启动 QEMU (仅x86_64)
make run                    # 构建并使用UEFI启动 QEMU
make clean                  # 清理构建文件
```

## 助项目一臂之力
欢迎提交 Issue 和 Pull Request 共同完善 Solum OS！

## 开源协议
本项目采用 GPL 3.0 开源协议。并使用了一些其他开源项目的源码    
  
## 引用及使用的开源项目

- Uinxed-Kernel: [https://github.com/ViudiraTech/Uinxed-Kernel](https://github.com/ViudiraTech/Uinxed-Kernel) (Apache 2.0)
- CoolPotOS: [https://github.com/plos-clan/CoolPotOS](https://github.com/plos-clan/CoolPotOS) (MIT License)
- NeoAetherOS: [https://github.com/aether-os-studio/naos](https://github.com/aether-os-studio/naos) (GPL 3.0)

>*Solum OS——筑就新世代通用操作系统*
