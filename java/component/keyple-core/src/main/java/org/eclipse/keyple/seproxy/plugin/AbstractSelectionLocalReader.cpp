#include "AbstractSelectionLocalReader.h"
#include "../exception/KeypleApplicationSelectionException.h"
#include "../exception/KeypleChannelStateException.h"
#include "../exception/KeypleIOReaderException.h"
#include "../message/SeRequest.h"
#include "../../util/ByteArrayUtils.h"
#include "ApduResponse.h"
#include "ApduRequest.h"
#include "SeSelector.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using ObservableReader = org::eclipse::keyple::seproxy::event::ObservableReader;
                    using KeypleApplicationSelectionException = org::eclipse::keyple::seproxy::exception::KeypleApplicationSelectionException;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using namespace org::eclipse::keyple::seproxy::message;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractSelectionLocalReader));

                    AbstractSelectionLocalReader::AbstractSelectionLocalReader(const std::string &pluginName, const std::string &readerName) : AbstractLocalReader(pluginName, readerName) {
                    }

                    std::shared_ptr<SelectionStatus> AbstractSelectionLocalReader::openLogicalChannel(std::shared_ptr<SeSelector> seSelector) throw(KeypleIOReaderException) {
                        std::shared_ptr<ApduResponse> fciResponse;
                        std::vector<char> atr = getATR();
                        bool selectionHasMatched = true;
                        std::shared_ptr<SelectionStatus> selectionStatus;

                        /** Perform ATR filtering if requested */
                        if (seSelector->getAtrFilter() != nullptr) {
                            if (atr.empty()) {
                                throw std::make_shared<KeypleIOReaderException>("Didn't get an ATR from the SE.");
                            }

                            if (logger->isTraceEnabled()) {
                                logger->trace("[{}] openLogicalChannel => ATR = {}", AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), ByteArrayUtils::toHex(atr));
                            }
                            if (!seSelector->getAtrFilter()->atrMatches(atr)) {
                                logger->info("[{}] openLogicalChannel => ATR didn't match. SELECTOR = {}, ATR = {}", AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), seSelector, ByteArrayUtils::toHex(atr));
                                selectionHasMatched = false;
                            }
                        }

                        /**
                         * Perform application selection if requested and if ATR filtering matched or was not
                         * requested
                         */
                        if (selectionHasMatched && seSelector->getAidSelector() != nullptr) {
                            std::shared_ptr<SeSelector::AidSelector> const aidSelector = seSelector->getAidSelector();
                            const std::vector<char> aid = aidSelector->getAidToSelect();
                            if (aid.empty()) {
                                throw std::invalid_argument("AID must not be null for an AidSelector.");
                            }
                            if (logger->isTraceEnabled()) {
                                logger->trace("[{}] openLogicalChannel => Select Application with AID = {}", AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), ByteArrayUtils::toHex(aid));
                            }

                            /*
                             * build a get response command the actual length expected by the SE in the get response
                             * command is handled in transmitApdu
                             */
                                    std::vector<char> selectApplicationCommand(6 + aid.size());
                                    selectApplicationCommand[0] = static_cast<char>(0x00); // CLA
                                    selectApplicationCommand[1] = static_cast<char>(0xA4); // INS
                                    selectApplicationCommand[2] = static_cast<char>(0x04); // P1: select by name
                            if (!aidSelector->isSelectNext()) {
                                selectApplicationCommand[3] = static_cast<char>(0x00); // P2: requests the first occurrence
                                    }
                                    else {
                                selectApplicationCommand[3] = static_cast<char>(0x02); // P2: requests the next occurrence
                                    }
                                    selectApplicationCommand[4] = static_cast<char>(aid.size()); // Lc
                                    System::arraycopy(aid, 0, selectApplicationCommand, 5, aid.size()); // data
                                    selectApplicationCommand[5 + aid.size()] = static_cast<char>(0x00); // Le

                                    /*
                             * we use here processApduRequest to manage case 4 hack. The successful status codes
                             * list for this command is provided.
                                     */
                            fciResponse = processApduRequest(std::make_shared<ApduRequest>("Internal Select Application", selectApplicationCommand, true, aidSelector->getSuccessfulSelectionStatusCodes()));

                            if (!fciResponse->isSuccessful()) {
                                logger->trace("[{}] openLogicalChannel => Application Selection failed. SELECTOR = {}", AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), aidSelector);
                            }
                            /*
                             * The ATR filtering matched or was not requested. The selection status is determined by
                             * the answer to the select application command.
                             */
                            selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(atr), fciResponse, fciResponse->isSuccessful());
                        }
                        else {
                            /*
                             * The ATR filtering didn't match or no AidSelector was provided. The selection status
                             * is determined by the ATR filtering.
                             */
                            std::vector<char> v;
                            selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(atr),
                                                                                std::make_shared<ApduResponse>(v, nullptr),
                                                                                selectionHasMatched);
                    }
                        return selectionStatus;
                    }
                }
            }
        }
    }
}
