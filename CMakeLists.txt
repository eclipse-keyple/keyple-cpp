#
# Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
#
# All rights reserved. This program and the accompanying materials are made available under the
# terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
# available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# set common output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add projects
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/keyple-core/src/seproxy )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/keyple-plugin-pcsc/src/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/keyple-example-pc/src/seproxy_testapplication ) 