#pragma once

#include <string>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "KeyReference.h"
#include "Logger.h"
#include "LoggerFactory.h"

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
                
                using namespace testing;

                class KeyReferenceTest : public std::enable_shared_from_this<KeyReferenceTest>
                {

                public:
                    virtual ~KeyReferenceTest(){};

                    int sample();
                };

            }
        }

