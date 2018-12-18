#include "CalypsoUtilities.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../generic/pc/src/main/java/org/eclipse/keyple/example/generic/pc/ReaderUtilities.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../postructure/CalypsoClassicInfo.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace calypso {
                    namespace common {
                        namespace transaction {
                            using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                            using CalypsoClassicInfo = org::eclipse::keyple::example::calypso::common::postructure::CalypsoClassicInfo;
                            using ReaderUtilities = org::eclipse::keyple::example::generic_Renamed::pc::ReaderUtilities;
                            using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                            using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                            using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                            using SeSelector = org::eclipse::keyple::transaction::SeSelector;
std::shared_ptr<java::util::Properties> CalypsoUtilities::properties;

                            CalypsoUtilities::StaticConstructor::StaticConstructor() {
                                                                    properties = std::make_shared<Properties>();
                                                            
                                                                    std::string propertiesFileName = "config.properties";
                                                            
                                                                    std::shared_ptr<InputStream> inputStream = CalypsoUtilities::typeid->getClassLoader().getResourceAsStream(propertiesFileName);
                                                            
                                                                    try {
                                                                        if (inputStream != nullptr) {
                                                                            properties->load(inputStream);
                                                                        }
                                                                        else {
//JAVA TO C++ CONVERTER TODO TASK: The following line could not be converted:
                                                                            throw java.io.FileNotFoundException("property file '" + propertiesFileName + "' not found!");
                                                                        }
                                                                    }
                                                                    catch (const FileNotFoundException &e) {
                                                                        e->printStackTrace();
                                                                    }
                                                                    catch (const IOException &e) {
                                                                        e->printStackTrace();
                                                                    }
                            }

CalypsoUtilities::StaticConstructor CalypsoUtilities::staticConstructor;

                            std::shared_ptr<SeReader> CalypsoUtilities::getDefaultPoReader(std::shared_ptr<SeProxyService> seProxyService) throw(KeypleBaseException) {
                                std::shared_ptr<SeReader> poReader = ReaderUtilities::getReaderByName(seProxyService, properties->getProperty("po.reader.regex"));

                                ReaderUtilities::setContactlessSettings(poReader);

                                return poReader;
                            }

                            std::shared_ptr<SeReader> CalypsoUtilities::getDefaultSamReader(std::shared_ptr<SeProxyService> seProxyService) throw(KeypleBaseException) {
                                std::shared_ptr<SeReader> samReader = ReaderUtilities::getReaderByName(seProxyService, properties->getProperty("sam.reader.regex"));

                                ReaderUtilities::setContactsSettings(samReader);

                                /*
                                 * Open logical channel for the SAM inserted in the reader
                                 *
                                 * (We expect the right is inserted)
                                 */
                                checkSamAndOpenChannel(samReader);

                                return samReader;
                            }

                            std::shared_ptr<EnumMap<PoTransaction::SamSettings, Byte>> CalypsoUtilities::getSamSettings() {
                                /* define the SAM parameters to provide when creating PoTransaction */
                                return std::make_shared<EnumMapAnonymousInnerClass>();
                            }

                            CalypsoUtilities::EnumMapAnonymousInnerClass::EnumMapAnonymousInnerClass() : java::util::EnumMap<org::eclipse::keyple::calypso::transaction::PoTransaction::SamSettings, Byte>(org::eclipse::keyple::calypso::transaction::PoTransaction::SamSettings::class) {
                            }

                            void CalypsoUtilities::checkSamAndOpenChannel(std::shared_ptr<SeReader> samReader) {
                                /*
                                 * check the availability of the SAM doing a ATR based selection, open its physical and
                                 * logical channels and keep it open
                                 */
                                std::shared_ptr<SeSelection> samSelection = std::make_shared<SeSelection>(samReader);

                                std::shared_ptr<SeSelector> samSelector = std::make_shared<SeSelector>(CalypsoClassicInfo::SAM_C1_ATR_REGEX, ChannelState::KEEP_OPEN, Protocol::ANY, "Selection SAM C1");

                                /* Prepare selector, ignore MatchingSe here */
                                samSelection->prepareSelection(samSelector);

                                try {
                                    if (!samSelection->processExplicitSelection()) {
                                        throw std::make_shared<IllegalStateException>("Unable to open a logical channel for SAM!");
                                    }
                                    else {
                                    }
                                }
                                catch (const KeypleReaderException &e) {
                                    throw std::make_shared<IllegalStateException>("Reader exception: " + e->what());

                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
