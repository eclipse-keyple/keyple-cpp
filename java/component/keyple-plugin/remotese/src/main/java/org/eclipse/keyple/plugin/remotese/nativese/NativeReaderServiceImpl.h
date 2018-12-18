#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include "NativeReaderService.h"
#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeProxyService; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderNotFoundException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }

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
                    namespace nativese {


                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;


                        /**
                         * Native Service to manage local reader and connect them to Remote Service
                         *
                         */
                        class NativeReaderServiceImpl : public std::enable_shared_from_this<NativeReaderServiceImpl>, public NativeReaderService, public DtoHandler, public ObservableReader::ReaderObserver {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            const std::shared_ptr<DtoSender> dtoSender;
                            const std::shared_ptr<SeProxyService> seProxyService;
                            // private final NseSessionManager nseSessionManager;

                            /**
                             * Constructor
                             * 
                             * @param dtoSender : Define which DTO sender will be called when a DTO needs to be sent.
                             */
                        public:
                            NativeReaderServiceImpl(std::shared_ptr<DtoSender> dtoSender);


                            /**
                             * HandleDTO from a TransportNode onDto() method will be called by the transportNode
                             * 
                             * @param node : network entry point that receives DTO
                             */
                            virtual void bindDtoEndpoint(std::shared_ptr<TransportNode> node);

                            /**
                             * Dispatch a Keyple DTO to the right Native Reader. {@link DtoHandler}
                             * 
                             * @param transportDto to be processed
                             * @return Keyple DTO to be sent back
                             */
                            std::shared_ptr<TransportDto> onDTO(std::shared_ptr<TransportDto> transportDto) override;


                            /**
                             * Connect a local reader to Remote SE Plugin {@link NativeReaderService}
                             * 
                             * @param clientNodeId : a chosen but unique terminal id (i.e AndroidDevice2)
                             * @param localReader : native reader to be connected
                             */
                            void connectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) throw(KeypleRemoteException) override;

                            void disconnectReader(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) throw(KeypleRemoteException) override;

                            /**
                             * Internal method to find a local reader by its name across multiple plugins
                             * 
                             * @param nativeReaderName : name of the reader to be found
                             * @return found reader if any
                             * @throws KeypleReaderNotFoundException if not reader were found with this name
                             */
                            virtual std::shared_ptr<ProxyReader> findLocalReader(const std::string &nativeReaderName) throw(KeypleReaderNotFoundException);

                            // NativeReaderService

                            /**
                             * Do not call this method directly This method is called by a Observable<{@link ReaderEvent}>
                             * 
                             * @param event event to be propagated to master device
                             */
                            void update(std::shared_ptr<ReaderEvent> event_Renamed) override;


                        };

                    }
                }
            }
        }
    }
}
