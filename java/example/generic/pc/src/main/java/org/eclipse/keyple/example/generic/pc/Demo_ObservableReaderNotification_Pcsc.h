#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {




                        class Demo_ObservableReaderNotification_Pcsc : public std::enable_shared_from_this<Demo_ObservableReaderNotification_Pcsc> {
                        public:
                            static const std::shared_ptr<void> waitBeforeEnd;

                            static void main(std::vector<std::string> &args) throw(std::runtime_error);
                        };

                    }
                }
            }
        }
    }
}
