#include "RmEventExecutor.h"
#include "../RemoteSePlugin.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../transport/json/JsonParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        namespace method {
                            using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;

                            RmEventExecutor::RmEventExecutor(std::shared_ptr<RemoteSePlugin> plugin) : plugin(plugin) {
                            }

                            std::shared_ptr<TransportDto> RmEventExecutor::execute(std::shared_ptr<TransportDto> transportDto) {
                                std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();

                                // parseResponse body
                                std::shared_ptr<ReaderEvent> event_Renamed = JsonParser::getGson()->fromJson(keypleDto->getBody(), ReaderEvent::typeid);

                                // dispatch reader event
                                plugin->onReaderEvent(event_Renamed, keypleDto->getSessionId());

                                // chain response if needed
                                // try {
                                // VirtualReader virtualReader =
                                // (VirtualReader) plugin.getReaderByRemoteName(keypleDto.getNativeReaderName());
                                //
                                // // chain response with a seRequest if needed
                                // if ((virtualReader.getSession()).hasSeRequestSet()) {
                                //
                                // // send back seRequestSet
                                // return transportDto
                                // .nextTransportDTO(new KeypleDto(RemoteMethod.READER_TRANSMIT.getName(),
                                // JsonParser.getGson()
                                // .toJson((virtualReader.getSession()).getSeRequestSet()),
                                // true, virtualReader.getSession().getSessionId()));
                                // } else {
                                // return transportDto.nextTransportDTO(KeypleDtoHelper.NoResponse());
                                // }
                                //
                                // } catch (KeypleReaderNotFoundException e) {
                                // return transportDto.nextTransportDTO(KeypleDtoHelper.ExceptionDTO(keypleDto.getAction(),
                                // e, keypleDto.getSessionId(), keypleDto.getNativeReaderName(),
                                // keypleDto.getVirtualReaderName(), keypleDto.getNodeId()));
                                // }

                                return transportDto->nextTransportDTO(KeypleDtoHelper::NoResponse());

                            }
                        }
                    }
                }
            }
        }
    }
}
