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

#pragma once

#include <string>
#include <vector>
#include <memory>

/* Plugin  - Stub */
#include "StubSecureElement.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace pc {
                        namespace stub {
                            namespace se {

                                using StubSecureElement =
                                    org::eclipse::keyple::plugin::stub::StubSecureElement;

                                /**
                                 * Simple contactless stub SE (no command)
                                 */
                                class StubSe2 : public StubSecureElement {

                                  public:
                                    static const std::string seProtocol;
                                    const std::string ATR_HEX = "3B8E800180318066409089120802830190000B";

                                    StubSe2();

                                    std::vector<char> getATR() override;

                                    std::string getSeProcotol() override;

                                  protected:
                                    std::shared_ptr<StubSe2> shared_from_this()
                                    {
                                        return std::static_pointer_cast<StubSe2>(StubSecureElement::shared_from_this());
                                    }
                                };

                            } // namespace se
                        }     // namespace stub
                    }         // namespace pc
                }             // namespace generic_Renamed
            }                 // namespace example
        }                     // namespace keyple
    }                         // namespace eclipse
} // namespace org
