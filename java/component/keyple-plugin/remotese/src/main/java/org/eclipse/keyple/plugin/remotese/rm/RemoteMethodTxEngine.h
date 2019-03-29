#pragma once

#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace rm { template<typename T>class RemoteMethodTx; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class TransportDto; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace exception { class KeypleRemoteException; } } } } } }

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
                    namespace rm {

                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;


                        /**
                         * Executor of RemoteMethodTx It manages : - send Dto - Parse Dto Responses
                         */
                        class RemoteMethodTxEngine : public std::enable_shared_from_this<RemoteMethodTxEngine>, public DtoHandler {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            // waiting transaction, supports only one at the time
                            std::shared_ptr<RemoteMethodTx> remoteMethodTx;

                            // Dto Sender
                            const std::shared_ptr<DtoSender> sender;

                        public:
                            RemoteMethodTxEngine(std::shared_ptr<DtoSender> sender);


                            /**
                             * Set Response to a RemoteMethod Invocation
                             * 
                             * @param message to be processed
                             * @return
                             */
                            std::shared_ptr<TransportDto> onDTO(std::shared_ptr<TransportDto> message) override;

                            /**
                             * Execute RemoteMethodTx
                             * 
                             * @param rm
                             */
                            virtual void register_Renamed(std::shared_ptr<RemoteMethodTx> rm) throw(KeypleRemoteException);

                        };

                    }
                }
            }
        }
    }
}
