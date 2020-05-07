/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "TestsUtilsStatusCodeGenerator.h"

namespace keyple {
namespace calypso {
namespace util {

std::vector<uint8_t> TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()
{
    return std::vector<uint8_t>{0x90, 0x00};
}

std::vector<uint8_t>
TestsUtilsStatusCodeGenerator::generateCommandForbiddenOnBinaryFilesStatusCode()
{
    return std::vector<uint8_t>{0x69, 0x81};
}

std::vector<uint8_t>
TestsUtilsStatusCodeGenerator::generateFileNotFoundStatusCode()
{
    return std::vector<uint8_t>{0x69, 0x82};
}

std::vector<uint8_t>
TestsUtilsStatusCodeGenerator::generateRecordNotFoundStatusCode()
{
    return std::vector<uint8_t>{0x6A, 0x83};
}

std::vector<uint8_t>
TestsUtilsStatusCodeGenerator::generateP2ValueNotSupportedStatusCode()
{
    return std::vector<uint8_t>{0x6B, 0x00};
}

std::vector<uint8_t>
TestsUtilsStatusCodeGenerator::generateLeValueIncorrectStatusCode()
{
    return std::vector<uint8_t>{0x6C, 0xFF};
}

std::vector<uint8_t>
TestsUtilsStatusCodeGenerator::generateAccessForbiddenStatusCode()
{
    return std::vector<uint8_t>{0x69, 0x85};
}
}
}
}
