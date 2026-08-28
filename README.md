# AnimeOS

AnimeOS is a bare-metal x86_64 operating system designed for high-performance linear framebuffer graphics rendering and video processing.

---

## Milestone 1: First Boot (Completed)

### Required Tools
- **Compiler:** LLVM Clang (`C:\Program Files\LLVM\bin\clang.exe`)
- **Linker:** LLVM LLD (`C:\Program Files\LLVM\bin\ld.lld.exe`)
- **Build System:** CMake `v4.4.0` (`C:\Program Files\CMake\bin\cmake.exe`) & Ninja (`python -m pip install ninja`)
- **Emulator:** QEMU x86_64 (`C:\Program Files\qemu\qemu-system-x86_64.exe`)
- **UEFI Firmware:** EDK2 x86_64 Firmware (`C:\Program Files\qemu\share\edk2-x86_64-code.fd`)
- **Bootloader:** Limine Bootloader Protocol (`v12.5.2` - `BOOTX64.EFI` & `limine.h`)

---

## Project Structure
```
AnimeOS/
├── CMakeLists.txt              # CMake build configuration for freestanding C x86_64 kernel
├── linker.ld                   # Linker script placing kernel in higher-half virtual memory
├── ARCHITECTURE_PLAN.md        # System architecture and incremental roadmap
├── README.md                   # Setup, build, and execution instructions
├── boot/
│   ├── BOOTX64.EFI             # Limine UEFI bootloader binary
│   └── limine.conf             # Limine configuration file
├── include/
│   └── limine.h                # Official Limine boot protocol header
└── src/
    └── kernel.c                # Freestanding C kernel entry point & UART serial logging
```

---

## Build Instructions

1. **Configure the Project:**
   ```cmd
   cmake -B build -G "Ninja" -DCMAKE_MAKE_PROGRAM="%LOCALAPPDATA%\Programs\Python\Python313\Scripts\ninja.exe"
   ```

2. **Compile Kernel and Assemble Boot Disk:**
   ```cmd
   cmake --build build
   ```

   **Output Artifacts:**
   - Freestanding Kernel ELF: `build/kernel.elf`
   - UEFI Boot Disk Directory: `build/disk_root/`

---

## QEMU Testing Command

To launch AnimeOS in QEMU with UEFI firmware and serial port output enabled:

```cmd
"C:\Program Files\qemu\qemu-system-x86_64.exe" -drive if=pflash,format=raw,readonly=on,file="C:\Program Files\qemu\share\edk2-x86_64-code.fd" -drive file=fat:rw:build/disk_root,format=raw -display none -monitor none -serial stdio
```

### Expected Output
```
BdsDxe: starting Boot0001 "UEFI QEMU HARDDISK QM00001 " ...

====================================================
  AnimeOS Kernel (x86_64) - Milestone 1: FIRST BOOT
  Status: AnimeOS booted successfully via Limine!
  Mode: 64-bit Long Mode (Freestanding C)
====================================================
```

---

## Success Criterion
QEMU boots the AnimeOS disk image through UEFI firmware, Limine loads `boot/kernel.elf`, transfers execution to `kernel_main()`, outputs the diagnostic banner to COM1 UART serial (`0x3F8`), and halts safely.

---

## Known Limitations (Milestone 1 Scope)
- Linear framebuffer graphics drawing is not yet initialized (scheduled for Milestone 3).
- Memory allocators (PMM / Heap `kmalloc`) are not yet implemented (scheduled for Milestone 4).
- Interrupts (IDT / APIC) and timer loops are not yet implemented (scheduled for Milestone 6).
