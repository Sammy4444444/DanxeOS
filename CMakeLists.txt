cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Set Clang compiler and LLVM Linker paths
set(LLVM_BIN_DIR "C:/Program Files/LLVM/bin")
set(CMAKE_C_COMPILER "${LLVM_BIN_DIR}/clang.exe")
set(CMAKE_ASM_COMPILER "${LLVM_BIN_DIR}/clang.exe")
set(CMAKE_LINKER "${LLVM_BIN_DIR}/ld.lld.exe")

project(AnimeOS C ASM)

# Freestanding C flags for x86_64 bare-metal kernel
set(CMAKE_C_FLAGS "-target x86_64-unknown-none-elf -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -fno-pie -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wall -Wextra")

# Direct linker command line using ld.lld.exe
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> -T ${CMAKE_SOURCE_DIR}/linker.ld -static -nostdlib --no-dynamic-linker <OBJECTS> -o <TARGET>")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(KERNEL_SOURCES
    src/kernel.c
)

add_executable(kernel.elf ${KERNEL_SOURCES})

set_target_properties(kernel.elf PROPERTIES
    ENABLE_EXPORTS OFF
    POSITION_INDEPENDENT_CODE OFF
)

# Prepare the UEFI boot disk directory structure
add_custom_target(boot_disk ALL
    DEPENDS kernel.elf
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/disk_root/EFI/BOOT"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/disk_root/boot"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/boot/BOOTX64.EFI" "${CMAKE_BINARY_DIR}/disk_root/EFI/BOOT/BOOTX64.EFI"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/boot/limine.conf" "${CMAKE_BINARY_DIR}/disk_root/EFI/BOOT/limine.conf"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/boot/limine.conf" "${CMAKE_BINARY_DIR}/disk_root/limine.conf"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:kernel.elf>" "${CMAKE_BINARY_DIR}/disk_root/boot/kernel.elf"
    COMMENT "Preparing AnimeOS UEFI boot disk directory structure"
)
