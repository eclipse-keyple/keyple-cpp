#include "AbstractSelectionLocalReader.h"
#include "../exception/KeypleApplicationSelectionException.h"
#include "../exception/KeypleChannelStateException.h"
#include "../exception/KeypleIOReaderException.h"
#include "../message/SeRequest.h"
#include "../../util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                    using KeypleApplicationSelectionException = org::eclipse::keyple::seproxy::exception::KeypleApplicationSelectionException;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using namespace org::eclipse::keyple::seproxy::message;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> AbstractSelectionLocalReader::logger = org::slf4j::LoggerFactory::getLogger(AbstractSelectionLocalReader::typeid);

                    AbstractSelectionLocalReader::AbstractSelectionLocalReader(const std::string &pluginName, const std::string &readerName) : AbstractLocalReader(pluginName, readerName) {
                    }

                    std::shared_ptr<SelectionStatus> AbstractSelectionLocalReader::openLogicalChannelAndSelect(std::shared_ptr<SeRequest::Selector> selector, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes) throw(KeypleChannelStateException, KeypleApplicationSelectionException, KeypleIOReaderException) {
                        char atr[], fci[] = nullptr;
                        bool selectionHasMatched;

                        if (!isLogicalChannelOpen()) {
                            /*
                             * init of the physical SE channel: if not yet established, opening of a new physical
                             * channel
                             */
                            if (!isPhysicalChannelOpen()) {
                                openPhysicalChannel();
                            }
                            if (!isPhysicalChannelOpen()) {
                                throw std::make_shared<KeypleChannelStateException>("Fail to open physical channel.");
                            }
                        }

                        /* get the ATR bytes */
                        atr = getATR();
                        if (logger->isTraceEnabled()) {
                            logger->trace("[{}] openLogicalChannelAndSelect => ATR: {}", this->getName(), ByteArrayUtils::toHex(atr));
                        }

                        /* selector may be null, in this case we consider the logical channel open */
                        if (selector != nullptr) {
                            if (std::dynamic_pointer_cast<SeRequest::AidSelector>(selector) != nullptr) {
                                std::vector<char> aid = (std::static_pointer_cast<SeRequest::AidSelector>(selector))->getAidToSelect();
                                if (aid.size() > 0) {
                                    if (logger->isTraceEnabled()) {
                                        logger->trace("[{}] openLogicalChannelAndSelect => Select Application with AID = {}", this->getName(), ByteArrayUtils::toHex(aid));
                                    }
                                    /*
                                     * build a get response command the actual length expected by the SE in the get
                                     * response command is handled in transmitApdu
                                     */
                                    std::vector<char> selectApplicationCommand(6 + aid.size());
                                    selectApplicationCommand[0] = static_cast<char>(0x00); // CLA
                                    selectApplicationCommand[1] = static_cast<char>(0xA4); // INS
                                    selectApplicationCommand[2] = static_cast<char>(0x04); // P1: select by name
                                    if (!(std::static_pointer_cast<SeRequest::AidSelector>(selector))->isSelectNext()) {
                                        selectApplicationCommand[3] = static_cast<char>(0x00); // P2: requests the first
                                                                                   // occurrence
                                    }
                                    else {
                                        selectApplicationCommand[3] = static_cast<char>(0x02); // P2: requests the next
                                                                                   // occurrence
                                    }
                                    selectApplicationCommand[4] = static_cast<char>(aid.size()); // Lc
                                    System::arraycopy(aid, 0, selectApplicationCommand, 5, aid.size()); // data
                                    selectApplicationCommand[5 + aid.size()] = static_cast<char>(0x00); // Le

                                    /*
                                     * we use here processApduRequest to manage case 4 hack. The successful status
                                     * codes list for this command is provided.
                                     */
                                    std::shared_ptr<ApduResponse> fciResponse = processApduRequest(std::make_shared<ApduRequest>("Internal Select Application", selectApplicationCommand, true, successfulSelectionStatusCodes));

                                    /* get the FCI bytes */
                                    fci = fciResponse->getBytes();

                                    if (fciResponse->isSuccessful()) {
                                        selectionHasMatched = true;
                                    }
                                    else {
                                        logger->trace("[{}] openLogicalChannelAndSelect => Application Selection failed. SELECTOR = {}", this->getName(), selector);
                                        selectionHasMatched = false;
                                    }
                                }
                                else {
                                    throw std::invalid_argument("AID must not be null for an AidSelector.");
                                }
                            }
                            else {
                                if ((std::static_pointer_cast<SeRequest::AtrSelector>(selector))->atrMatches(atr)) {
                                    selectionHasMatched = true;
                                }
                                else {
                                    logger->trace("[{}] openLogicalChannelAndSelect => ATR Selection failed. SELECTOR = {}", this->getName(), selector);
                                    selectionHasMatched = false;
                                }
                            }
                        }
                        else {
                            selectionHasMatched = true;
                        }

                        return std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(atr), std::make_shared<ApduResponse>(fci, nullptr), selectionHasMatched);
                    }
                }
            }
        }
    }
}
