#pragma once

#include "../../../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubSecureElement.h"
#include <string>
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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {
                        namespace stub {
                            namespace se {


                                using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;

                                /**
                                 * Simple contact stub SE (no command)
                                 */
                                class StubSe1 : public StubSecureElement {

                                public:
                                    static const std::string seProtocol;
                                    const std::string ATR_HEX = "3B3F9600805A0080C120000012345678829000"; // serial number : 12345678

                                    StubSe1();

                                    std::vector<char> getATR() override;

                                    std::string getSeProcotol() override;



protected:
                                    std::shared_ptr<StubSe1> shared_from_this() {
                                        return std::static_pointer_cast<StubSe1>(org.eclipse.keyple.plugin.stub.StubSecureElement::shared_from_this());
                                    }
                                };

                            }
                        }
                    }
                }
            }
        }
    }
}
