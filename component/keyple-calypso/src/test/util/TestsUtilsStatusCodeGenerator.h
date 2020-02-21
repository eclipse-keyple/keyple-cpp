#pragma once

#include <vector>
#include <memory>

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/

        namespace keyple {
            namespace calypso {
                namespace util {

                    class TestsUtilsStatusCodeGenerator : public std::enable_shared_from_this<TestsUtilsStatusCodeGenerator> {

                    public:
                        static std::vector<char> generateSuccessfulStatusCode();

                        static std::vector<char> generateCommandForbiddenOnBinaryFilesStatusCode();

                        static std::vector<char> generateFileNotFoundStatusCode();

                        static std::vector<char> generateRecordNotFoundStatusCode();

                        static std::vector<char> generateP2ValueNotSupportedStatusCode();

                        static std::vector<char> generateLeValueIncorrectStatusCode();

                        static std::vector<char> generateAccessForbiddenStatusCode();
                    };

                }
            }
        }
