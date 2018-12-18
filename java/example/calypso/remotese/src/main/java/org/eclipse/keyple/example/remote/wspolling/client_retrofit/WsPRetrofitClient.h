#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }

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
                namespace remote {
                    namespace wspolling {
                        namespace client_retrofit {


                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using retrofit2::Call;

                            class WsPRetrofitClient {

                            public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Headers({"Accept: application/json", "Content-Type: application/json; charset=UTF-8"}) @GET("polling") retrofit2.Call<org.eclipse.keyple.plugin.remotese.transport.KeypleDto> getPolling(@Query("clientNodeId") String nodeId);
                                virtual std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> getPolling(const std::string &nodeId) = 0;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Headers({"Accept: application/json", "Content-Type: application/json; charset=UTF-8"}) @POST("keypleDTO") retrofit2.Call<org.eclipse.keyple.plugin.remotese.transport.KeypleDto> postDto(@Body org.eclipse.keyple.plugin.remotese.transport.KeypleDto keypleDto);
                                virtual std::shared_ptr<Call<std::shared_ptr<KeypleDto>>> postDto(std::shared_ptr<KeypleDto> keypleDto) = 0;

                            };

                        }
                    }
                }
            }
        }
    }
}
