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

std::vector<char> TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()
{
    return std::vector<char>{static_cast<char>(0x90), 0x00};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateCommandForbiddenOnBinaryFilesStatusCode()
{
    return std::vector<char>{static_cast<char>(0x69), static_cast<char>(0x81)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateFileNotFoundStatusCode()
{
    return std::vector<char>{static_cast<char>(0x69), static_cast<char>(0x82)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateRecordNotFoundStatusCode()
{
    return std::vector<char>{static_cast<char>(0x6A), static_cast<char>(0x83)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateP2ValueNotSupportedStatusCode()
{
    return std::vector<char>{static_cast<char>(0x6B), 0x00};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateLeValueIncorrectStatusCode()
{
    return std::vector<char>{static_cast<char>(0x6C), static_cast<char>(0xFF)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateAccessForbiddenStatusCode()
{
    return std::vector<char>{static_cast<char>(0x69), static_cast<char>(0x85)};
}
}
}
}
