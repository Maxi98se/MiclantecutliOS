SET(CMAKE_SYSTEM_NAME               Generic)
SET(CMAKE_SYSTEM_VERSION            0.1)
SET(CMAKE_SYSTEM_PROCESSOR          i386)

# Optionally reduce compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE         STATIC_LIBRARY)

set(CMAKE_AR                        $ENV{PREFIX}/bin/$ENV{TARGET}-gcc-ar)
set(CMAKE_ASM_COMPILER              nasm)
set(CMAKE_C_COMPILER                $ENV{PREFIX}/bin/$ENV{TARGET}-gcc)
set(CMAKE_LINKER                    $ENV{PREFIX}/bin/$ENV{TARGET}-ld)
set(CMAKE_OBJCOPY                   $ENV{PREFIX}/bin/$ENV{TARGET}-objcopy CACHE INTERNAL "")
set(CMAKE_RANLIB                    $ENV{PREFIX}/bin/$ENV{TARGET}-ranlib CACHE INTERNAL "")
set(CMAKE_SIZE                      $ENV{PREFIX}/bin/$ENV{TARGET}-size CACHE INTERNAL "")
set(CMAKE_STRIP                     $ENV{PREFIX}/bin/$ENV{TARGET}-strip CACHE INTERNAL "")

set(CMAKE_ASM_NASM_LINK_EXECUTABLE "${CMAKE_LINKER} <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_C_FLAGS                   "-ffreestanding -mno-red-zone -nostdlib" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-O2 -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM     NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY     ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE     ONLY)
