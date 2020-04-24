# The name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)
SET(TOOLCHAIN_FILE "clang-windows.cmake")

# Path
#SET(CLANG_TOOLCHAIN_DIR /usr)
#SET(CLANG_COMPILER_DIR  ${CLANG_TOOLCHAIN_DIR}/bin)
#SET(CLANG_SYSROOT_DIR   ${CLANG_TOOLCHAIN_DIR}/sysroot)

# Which C and C++ compiler to use
SET(CMAKE_C_COMPILER   cl.exe)
SET(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
SET(CMAKE_CXX_COMPILER cl.exe)
#SET(CMAKE_AR           ${TOOLCHAIN_PREFIX}ar)
#SET(CMAKE_RANLIB       ${TOOLCHAIN_PREFIX}ranlib)

# Here is the target environment located
#SET(CMAKE_FIND_ROOT_PATH  ${CLANG_SYSROOT_DIR})

# Adjust the default behaviour of the FIND_XXX() commands:
#   search headers and libraries in the target environment,
#   search programs in the host environment
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

# Compiler flags
SET(CMAKE_CXX_FLAGS "")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")     # warning level
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")     # debug symbols
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")   # exception handling
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4290")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4309")

# Linker
#SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${CLANG_SYSROOT_DIR}")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${CLANG_SYSROOT_DIR} -L${CLANG_SYSROOT_DIR}")

# Libraries type
SET(LIBRARY_TYPE SHARED)

