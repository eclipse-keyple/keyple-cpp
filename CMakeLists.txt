#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#include(toolchain.cmake)

SET(CMAKE_LEGACY_CYGWIN_WIN32 0)
SET(CMAKE_CXX_STANDARD 11)

# Compilers¬
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)

project(Project)

IF(APPLE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -Wall -Wextra -Wdeprecated -Wno-overloaded-virtual -pedantic -pedantic-errors -std=c++11")
ELSE(APPLE)
	IF(NOT TOOLCHAIN_FILE)
		MESSAGE(FATAL_ERROR "Toolchain file not specified")
	ENDIF()
ENDIF(APPLE)

# Set common output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(GOOGLETEST_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add projects
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/component/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example/)
