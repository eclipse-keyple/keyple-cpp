#pragma once

#include <string>
#include "exceptionhelper.h"
#include <memory>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class Selector; } } } } }

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

                            using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                            using namespace com::google::gson;

                            class GsonSelectorTypeAdapter : public std::enable_shared_from_this<GsonSelectorTypeAdapter>, public JsonDeserializer<std::shared_ptr<SeRequest::Selector>>, public JsonSerializer<std::shared_ptr<SeRequest::Selector>> {

                            public:
                                std::shared_ptr<JsonElement> serialize(std::shared_ptr<SeRequest::Selector> src, std::shared_ptr<Type> typeOfSrc, std::shared_ptr<JsonSerializationContext> context) override;

                                std::shared_ptr<SeRequest::Selector> deserialize(std::shared_ptr<JsonElement> json, std::shared_ptr<Type> typeOfT, std::shared_ptr<JsonDeserializationContext> context) throw(JsonParseException) override;
                            };

                        }
                    }
                }
            }
        }
    }
}
