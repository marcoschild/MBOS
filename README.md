# MBOS – The Marco Built Operating System

**MBOS** is a minimalist, lightweight operating system designed from the ground up for creators, writers, and developers who value clarity, speed, and control. Built without reliance on existing Linux distributions, MBOS aims to offer a clean foundation for custom system design and low-level exploration.

---

## 🧠 Core Philosophy

- 🧩 **Minimalism** – Only the essentials. No bloat.
- 📚 **Designed for Writing** – Distraction-free terminal, clean fonts, Markdown-native notes
- ⚙️ **Built from Scratch** – No dependency on Linux, busybox, or GNU coreutils
- 💡 **Hackable** – Modular components for scheduling, memory management, IO
- 🔐 **Secure by Simplicity** – Smaller surface, easier auditing

---

## 📁 Repo Structure

```
MBOS/
├── src/            # Core OS source files (bootloader, kernel, drivers)
├── docs/           # Developer documentation and build instructions
├── scripts/        # Build tools and deployment scripts
├── LICENSE
├── README.md
└── Makefile
```

---

## 🚀 Getting Started

> 🚧 MBOS is under active development and not yet production-ready.

To build the current boot image:
```bash
make all
```

To run in QEMU:
```bash
qemu-system-x86_64 -drive format=raw,file=build/mbos.img
```

---

## 🛠 Planned Features

- Custom memory manager
- Minimal shell written in C
- FAT/EXT2 file system support
- Windowless GUI for writing & notes
- Native text editor
- Custom bootloader

---

## 🧪 Development Tools

- Assembly (NASM)
- C (freestanding)
- QEMU (testing)
- GNU Make

---

## 📜 License

MIT – Free to use, modify, and learn from.
