#pragma once

#include <string>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }

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

                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                        using com::google::gson::JsonObject;

                        /**
                         * Utility class to manipulate KeypleDto
                         */
                        class KeypleDtoHelper : public std::enable_shared_from_this<KeypleDtoHelper> {


                        public:
                            static std::string toJson(std::shared_ptr<KeypleDto> keypleDto);

                            static std::shared_ptr<KeypleDto> fromJson(const std::string &json);

                            static std::shared_ptr<KeypleDto> fromJsonObject(std::shared_ptr<JsonObject> jsonObj);

                            static std::shared_ptr<KeypleDto> NoResponse();

                            static std::shared_ptr<KeypleDto> ExceptionDTO(const std::string &action, std::runtime_error exception, const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId);

                            static Boolean isNoResponse(std::shared_ptr<KeypleDto> dto);

                        private:
                            static Boolean isKeypleDTO(const std::string &json);

                        public:
                            static std::shared_ptr<KeypleDto> ACK();

                            static Boolean isKeypleDTO(std::shared_ptr<JsonObject> json);

                            static Boolean containsException(std::shared_ptr<KeypleDto> keypleDto);


                        };

                    }
                }
            }
        }
    }
}
