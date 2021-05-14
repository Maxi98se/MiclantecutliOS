set(BOOT-SRC
    boot.asm
)
set(KERNEL-SRC
    kernel.asm
    kernel.c
)


cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE  ON)
set(CMAKE_TOOLCHAIN_FILE    ${CMAKE_SOURCE_DIR}/toolchains/toolchain-$ENV{TARGET}.cmake)

project(Miclantecutli C ASM_NASM)

add_executable(OS-BOOT ${BOOT-SRC})
target_compile_options(OS-BOOT BEFORE PRIVATE SHELL: -F dwarf -g)
target_link_options(OS-BOOT BEFORE PRIVATE LINKER: -Ttext=7c00 --oformat binary)

add_executable(OS-KERNEL ${KERNEL-SRC})
target_link_options(OS-KERNEL BEFORE PRIVATE LINKER: -Ttext=7e00 -Wl,--oformat=binary)