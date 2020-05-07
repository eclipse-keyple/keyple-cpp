#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

CONFIGURE_FILE(CMakeLists.txt.in ${GOOGLETEST_DIRECTORY}/googletest-download/CMakeLists.txt)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${GOOGLETEST_DIRECTORY}/googletest-download
)

IF(result)
    MESSAGE(FATAL_ERROR "CMake step for googletest failed: ${result}")
ENDIF()

EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${GOOGLETEST_DIRECTORY}/googletest-download
)

IF(result)
    MESSAGE(FATAL_ERROR "Build step for googletest failed: ${result}")
ENDIF()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
#SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
ADD_SUBDIRECTORY(${GOOGLETEST_DIRECTORY}/googletest-src
				 ${GOOGLETEST_DIRECTORY}/googletest-build
                 EXCLUDE_FROM_ALL
)

# Add projects
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/keyple-calypso/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/keyple-common/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/keyple-core/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/keyple-plugin/)
