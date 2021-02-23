# The name of the target operating system
SET(CMAKE_SYSTEM_NAME Darwin)
SET(TOOLCHAIN_FILE "ios-macos.cmake")

# Path
SET(CLANG_TOOLCHAIN_DIR "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr")
SET(CLANG_COMPILER_DIR  ${CLANG_TOOLCHAIN_DIR}/bin)
#SET(CLANG_SYSROOT_DIR   ${CLANG_TOOLCHAIN_DIR}/sysroot)

# Which C and C++ compiler to use
SET(CMAKE_C_COMPILER   ${CLANG_COMPILER_DIR}/clang)
SET(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
SET(CMAKE_CXX_COMPILER ${CLANG_COMPILER_DIR}/clang++)
#SET(CMAKE_AR           ${TOOLCHAIN_PREFIX}ar)
#SET(CMAKE_RANLIB       ${TOOLCHAIN_PREFIX}ranlib)

SET(XCODE_IOS_PLATFORM iphoneos)
SET(IOS_ARCH arm64)
SET(CMAKE_OSX_ARCHITECTURES ${IOS_ARCH} CACHE STRING "Build architecture for iOS")


# If user did not specify the SDK root to use, then query xcodebuild for it.
EXECUTE_PROCESS(
    COMMAND xcodebuild -version -sdk ${XCODE_IOS_PLATFORM} Path
    OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
# Find (Apple's) libtool.
EXECUTE_PROCESS(
    COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find libtool
    OUTPUT_VARIABLE IOS_LIBTOOL
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
# Get the SDK version information.
EXECUTE_PROCESS(
    COMMAND xcodebuild -sdk ${CMAKE_OSX_SYSROOT} -version SDKVersion
    OUTPUT_VARIABLE IOS_SDK_VERSION
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Configure libtool to be used instead of ar + ranlib to build static libraries.
# This is required on Xcode 7+, but should also work on previous versions of
# Xcode.
SET(CMAKE_C_CREATE_STATIC_LIBRARY   "${IOS_LIBTOOL} -static -o <TARGET> <LINK_FLAGS> <OBJECTS> ")
SET(CMAKE_CXX_CREATE_STATIC_LIBRARY "${IOS_LIBTOOL} -static -o <TARGET> <LINK_FLAGS> <OBJECTS> ")

# Here is the target environment located
# Set the find root to the iOS developer roots and to user defined paths.
SET(CMAKE_FIND_ROOT_PATH ${CMAKE_IOS_DEVELOPER_ROOT} ${CMAKE_OSX_SYSROOT}
    ${CMAKE_PREFIX_PATH} CACHE STRING  "iOS find search path root" FORCE)
# Default to searching for frameworks first.
SET(CMAKE_FIND_FRAMEWORK FIRST)
# Set up the default search directories for frameworks.
SET(CMAKE_SYSTEM_FRAMEWORK_PATH
    ${CMAKE_OSX_SYSROOT}/System/Library/Frameworks
    ${CMAKE_OSX_SYSROOT}/System/Library/PrivateFrameworks
    ${CMAKE_OSX_SYSROOT}/Developer/Library/Frameworks)

# Adjust the default behaviour of the FIND_XXX() commands:
#   search headers and libraries in the target environment,
#   search programs in the host environment
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

# Compiler flags
SET(CMAKE_CXX_FLAGS "")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdeprecated")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mios-version-min=13.0")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-abi-version=2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")

SET(CMAKE_OBJCXX_FLAGS "${CMAKE_OBJCXX_FLAGS} -fembed-bitcode")

# Linker
#SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${CLANG_SYSROOT_DIR}")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${CLANG_SYSROOT_DIR} -L${CLANG_SYSROOT_DIR}")
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mios-version-min=13.0")
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Wl,-search_paths_first")

# Libraries type
SET(LIBRARY_TYPE STATIC)

