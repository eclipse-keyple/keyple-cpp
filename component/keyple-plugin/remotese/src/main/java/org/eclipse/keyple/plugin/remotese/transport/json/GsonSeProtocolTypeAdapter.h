#pragma once

#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocol; } } } } }

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
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace json {


                            using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                            using namespace com::google::gson;

                            class GsonSeProtocolTypeAdapter : public std::enable_shared_from_this<GsonSeProtocolTypeAdapter>, public JsonDeserializer<std::shared_ptr<SeProtocol>>, public JsonSerializer<std::shared_ptr<SeProtocol>> {

                            public:
                                std::shared_ptr<JsonElement> serialize(std::shared_ptr<SeProtocol> src, std::shared_ptr<Type> typeOfSrc, std::shared_ptr<JsonSerializationContext> context) override;

                                std::shared_ptr<SeProtocol> deserialize(std::shared_ptr<JsonElement> json, std::shared_ptr<Type> typeOfT, std::shared_ptr<JsonDeserializationContext> context) throw(JsonParseException) override;


                            };

                        }
                    }
                }
            }
        }
    }
}
