#pragma once

#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class RemoteSePlugin; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeProxyService; } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace factory { class TransportNode; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class TransportDto; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderNotFoundException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReader; } } } } } }

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
                    namespace pluginse {

                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::factory::TransportNode;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;


                        /**
                         * Service to setDtoSender a RSE Plugin to a Transport Node
                         */
                        class VirtualReaderService : public std::enable_shared_from_this<VirtualReaderService>, public DtoHandler {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            const std::shared_ptr<DtoSender> dtoSender;
                            const std::shared_ptr<RemoteSePlugin> plugin;

                            /**
                             * Build a new VirtualReaderService, Entry point for incoming DTO in Master Manages
                             * RemoteSePlugin lifecycle Manages Master Session Dispatch KeypleDTO
                             *
                             * @param seProxyService : SeProxyService
                             * @param dtoSender : outgoing node to send Dto to Slave
                             */
                        public:
                            VirtualReaderService(std::shared_ptr<SeProxyService> seProxyService, std::shared_ptr<DtoSender> dtoSender);

                            /**
                             * Set this service as the Dto Dispatcher in your {@link TransportNode}
                             * 
                             * @param node : incoming Dto point
                             */
                            virtual void bindDtoEndpoint(std::shared_ptr<TransportNode> node);

                            /**
                             * Retrieve the Rse Plugin
                             * 
                             * @return the Remote Se Plugin managing the Virtual Readers
                             */
                            virtual std::shared_ptr<RemoteSePlugin> getPlugin();

                            /**
                             * Handles incoming transportDTO
                             * 
                             * @param transportDto an incoming TransportDto (embeds a KeypleDto)
                             * @return a Response transportDto (can be a NoResponse KeypleDto)
                             */
                            std::shared_ptr<TransportDto> onDTO(std::shared_ptr<TransportDto> transportDto) override;



                            /**
                             * Attach a SeRequestSet to keypleDto response object if a seRequestSet object is pending in the
                             * virtual reader session If not, returns the same keypleDto
                             *
                             * @param transportDto : response to be sent
                             * @return enriched response
                             */
                            // private TransportDto isSeRequestToSendBack(TransportDto transportDto) {
                            // TransportDto out = null;
                            // try {
                            // // retrieve reader by session
                            // VirtualReader virtualReader = (VirtualReader) plugin
                            // .getReaderByRemoteName(transportDto.getKeypleDTO().getNativeReaderName());
                            //
                            // if ((virtualReader.getRmTxEngine()).hasSeRequestSet()) {
                            //
                            // // send back seRequestSet
                            // out = transportDto
                            // .nextTransportDTO(new KeypleDto(RemoteMethod.READER_TRANSMIT.getName(),
                            // JsonParser.getGson()
                            // .toJson((virtualReader.getSession()).getSeRequestSet()),
                            // true, virtualReader.getSession().getSessionId()));
                            // } else {
                            // // no response
                            // out = transportDto.nextTransportDTO(KeypleDtoHelper.NoResponse());
                            // }
                            //
                            // } catch (KeypleReaderNotFoundException e) {
                            // logger.debug("Reader was not found by session", e);
                            // KeypleDto keypleDto = transportDto.getKeypleDTO();
                            // out = transportDto.nextTransportDTO(KeypleDtoHelper.ExceptionDTO(keypleDto.getAction(),
                            // e, keypleDto.getSessionId(), keypleDto.getNativeReaderName(),
                            // keypleDto.getVirtualReaderName(), keypleDto.getNodeId()));
                            // }
                            //
                            // return out;
                            // }


                            /**
                             * Retrieve reader by its session Id
                             * 
                             * @param sessionId
                             * @return VirtualReader matching the sessionId
                             * @throws KeypleReaderNotFoundException
                             */
                        private:
                            std::shared_ptr<VirtualReader> getReaderBySessionId(const std::string &sessionId) throw(KeypleReaderNotFoundException, KeypleReaderException);

                        };

                    }
                }
            }
        }
    }
}
