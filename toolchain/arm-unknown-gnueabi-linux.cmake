# The name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)
SET(TOOLCHAIN_FILE "arm-unknown-gnueabi-linux.cmake")

# Path
SET(ARM_TOOLCHAIN_DIR /home/amunsch/toolchain/yt/imx)
SET(ARM_COMPILER_DIR  ${ARM_TOOLCHAIN_DIR}/bin)
SET(ARM_SYSROOT_DIR   ${ARM_TOOLCHAIN_DIR}/arm-unknown-linux-gnueabi/sysroot)

# Which C and C++ compiler to use
SET(CMAKE_C_COMPILER   ${ARM_COMPILER_DIR}/arm-unknown-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER ${ARM_COMPILER_DIR}/arm-unknown-linux-gnueabi-g++) 

# Here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  ${ARM_SYSROOT_DIR}) 

# Adjust the default behaviour of the FIND_XXX() commands:
#   search headers and libraries in the target environment,
#   search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

# Linker
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${ARM_SYSROOT_DIR}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${ARM_SYSROOT_DIR} -L${ARM_SYSROOT_DIR}")

