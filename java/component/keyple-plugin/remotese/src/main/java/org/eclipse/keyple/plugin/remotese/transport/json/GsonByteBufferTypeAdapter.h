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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace json {

                            using namespace com::google::gson;

                            class GsonByteBufferTypeAdapter : public std::enable_shared_from_this<GsonByteBufferTypeAdapter>, public JsonDeserializer<std::vector<char>>, public JsonSerializer<std::vector<char>> {

                            public:
                                std::shared_ptr<JsonElement> serialize(std::vector<char> &src, std::shared_ptr<Type> typeOfSrc, std::shared_ptr<JsonSerializationContext> context) override;

                                std::vector<char> deserialize(std::shared_ptr<JsonElement> json, std::shared_ptr<Type> typeOfT, std::shared_ptr<JsonDeserializationContext> context) throw(JsonParseException) override;


                            };

                        }
                    }
                }
            }
        }
    }
}
