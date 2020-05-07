#******************************************************************************
#* Copyright (c) 2018 Calypso Networks Association                            *
#* https://www.calypsonet-asso.org/                                           *
#*                                                                            *
#* See the NOTICE file(s) distributed with this work for additional           *
#* information regarding copyright ownership.                                 *
#*                                                                            *
#* This program and the accompanying materials are made available under the   *
#* terms of the Eclipse Public License 2.0 which is available at              *
#* http://www.eclipse.org/legal/epl-2.0                                       *
#*                                                                            *
#* SPDX-License-Identifier: EPL-2.0                                           *
#******************************************************************************/

PROJECT(ExamplePc C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

SET(PACKAGE_NAME "ExamplePc")
SET(PACKAGE_VERSION ${VERSION})
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

SET(CMAKE_MACOSX_RPATH 1)
SET(CMAKE_CXX_STANDARD 11)

INCLUDE(CMakeLists.txt.demo_observablereadernotification_pcsc)
INCLUDE(CMakeLists.txt.demo_observablereadernotification_stub)
INCLUDE(CMakeLists.txt.demo_seprotocoldetection_pcsc)
INCLUDE(CMakeLists.txt.demo_seprotocoldetection_stub)
INCLUDE(CMakeLists.txt.usecase_generic1_explicitselectionaid_pcsc)
INCLUDE(CMakeLists.txt.usecase_generic2_defaultselectionnotification_pcsc)
INCLUDE(CMakeLists.txt.usecase_generic3_groupedmultiselection_pcsc)
INCLUDE(CMakeLists.txt.usecase_generic4_sequentialmultiselection_pcsc)
