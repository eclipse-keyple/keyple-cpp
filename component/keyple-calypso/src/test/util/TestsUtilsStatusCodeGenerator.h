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

#pragma once

#include <vector>
#include <memory>

namespace keyple {
namespace calypso {
namespace util {

class TestsUtilsStatusCodeGenerator
: public std::enable_shared_from_this<TestsUtilsStatusCodeGenerator> {

public:
    static std::vector<uint8_t> generateSuccessfulStatusCode();

    static std::vector<uint8_t> generateCommandForbiddenOnBinaryFilesStatusCode();

    static std::vector<uint8_t> generateFileNotFoundStatusCode();

    static std::vector<uint8_t> generateRecordNotFoundStatusCode();

    static std::vector<uint8_t> generateP2ValueNotSupportedStatusCode();

    static std::vector<uint8_t> generateLeValueIncorrectStatusCode();

    static std::vector<uint8_t> generateAccessForbiddenStatusCode();
};

}
}
}
