# The name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)
SET(TOOLCHAIN_FILE "gcc-4.8.3-linux.cmake")

# Path
SET(GCC_TOOLCHAIN_DIR /home/amunsch/Downloads/gcc-4.8.3)
SET(GCC_COMPILER_DIR  ${GCC_TOOLCHAIN_DIR}/bin)
#SET(GCC_SYSROOT_DIR   ${GCC_TOOLCHAIN_DIR}/sysroot)

# Which C and C++ compiler to use
SET(CMAKE_C_COMPILER   ${GCC_COMPILER_DIR}/gcc)
SET(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
SET(CMAKE_CXX_COMPILER ${GCC_COMPILER_DIR}/g++)
SET(CMAKE_AR           ${TOOLCHAIN_PREFIX}ar)
SET(CMAKE_RANLIB       ${TOOLCHAIN_PREFIX}ranlib)

# Here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  ${GCC_SYSROOT_DIR})

# Adjust the default behaviour of the FIND_XXX() commands:
#   search headers and libraries in the target environment,
#   search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

# Linker
#SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${GCC_SYSROOT_DIR}")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${GCC_SYSROOT_DIR} -L${GCC_SYSROOT_DIR}")

