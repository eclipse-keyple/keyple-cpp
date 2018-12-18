#pragma once

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


                            using com::google::gson::Gson;
                            using com::google::gson::JsonObject;

                            /**
                             * Json Parser for Keyple DTO and Keyple DTO fields
                             */
                            class JsonParser : public std::enable_shared_from_this<JsonParser> {

                            public:
                                static std::shared_ptr<Gson> getGson();

                                static Boolean isSeRequestSet(std::shared_ptr<JsonObject> obj);

                            };

                        }
                    }
                }
            }
        }
    }
}
