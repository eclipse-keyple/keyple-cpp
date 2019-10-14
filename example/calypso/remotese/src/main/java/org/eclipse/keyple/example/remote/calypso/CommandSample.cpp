#include "CommandSample.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/RemoteSePlugin.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReader.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/builder/ReadRecordsCmdBuild.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace calypso {
                        using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                        using ReadRecordsCmdBuild = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                        using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                        using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using namespace org::eclipse::keyple::seproxy::message;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;

                        void CommandSample::transmit(std::shared_ptr<Logger> logger, const std::string &remoteReaderName) {
                            try {


                                logger->info("--- Execute a Blocking transmit --- ");

                                // get the reader by its name
                                std::shared_ptr<VirtualReader> * const reader = std::static_pointer_cast<VirtualReader>((std::static_pointer_cast<RemoteSePlugin>(SeProxyService::getInstance()->getPlugins()->first()))->getReaderByRemoteName(remoteReaderName));

                                std::string poAid = "A000000291A000000191";

                                // build 1st seRequestSet with keep channel open to true
                                std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "Hoplink EF T2Environment");

                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;
                                poApduRequestList = Arrays::asList(poReadRecordCmd_T2Env->getApduRequest());
                                std::shared_ptr<SeRequest::Selector> * const selector = std::make_shared<SeRequest::AidSelector>(ByteArrayUtils::fromHex(poAid));
                                std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(selector, poApduRequestList, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, nullptr);

                                // SYNC transmit seRequestSet to Reader With Callback function
                                std::shared_ptr<SeResponseSet> seResponseSet = reader->transmitSet(std::make_shared<SeRequestSet>(seRequest));

                                logger->info("Received SYNCHRONOUSLY a 1rt SeResponseSet - isSuccessful : {}", seResponseSet);

                                // build 1st seRequestSet with keep channel open to true
                                std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env2 = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "Hoplink EF T2Environment");

                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList2;
                                poApduRequestList2 = Arrays::asList(poReadRecordCmd_T2Env2->getApduRequest());

                                std::shared_ptr<SeRequest> seRequest2 = std::make_shared<SeRequest>(selector, poApduRequestList2, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, nullptr);

                                // SYNC transmit seRequestSet to Reader With Callback function
                                std::shared_ptr<SeResponseSet> seResponseSet2 = reader->transmitSet(std::make_shared<SeRequestSet>(seRequest2));

                                logger->info("Received SYNCHRONOUSLY a 2nd SeResponseSet - isSuccessful : {}", seResponseSet2);


                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                            }

                        }
                    }
                }
            }
        }
    }
}
