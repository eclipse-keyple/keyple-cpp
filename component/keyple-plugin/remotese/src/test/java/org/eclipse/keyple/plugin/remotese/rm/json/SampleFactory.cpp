#include "SampleFactory.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/DefaultSelectionRequest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactsProtocols.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace rm {
                        namespace json {
                            using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                            using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                            using DefaultSelectionRequest = org::eclipse::keyple::seproxy::event_Renamed::DefaultSelectionRequest;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using namespace org::eclipse::keyple::seproxy::message;
                            using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                            using ContactsProtocols = org::eclipse::keyple::seproxy::protocol::ContactsProtocols;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                            std::shared_ptr<KeypleBaseException> SampleFactory::getAStackedKeypleException() {
                                return std::make_shared<KeypleReaderException>("Keyple Reader Exception", std::make_shared<IOException>("IO Error", std::make_shared<IOException>("IO Error2", std::runtime_error("sdfsdf"))));
                            }

                            std::shared_ptr<KeypleBaseException> SampleFactory::getASimpleKeypleException() {
                                return std::make_shared<KeypleReaderException>("Keyple Reader Exception");
                            }

                            std::shared_ptr<DefaultSelectionRequest> SampleFactory::getSelectionRequest() {
                                return std::make_shared<DefaultSelectionRequest>(getASeRequestSet_ISO14443_4());
                            }

                            ObservableReader::NotificationMode SampleFactory::getNotificationMode() {
                                return ObservableReader::NotificationMode::ALWAYS;
                            }

                            std::shared_ptr<SeRequestSet> SampleFactory::getASeRequestSet_ISO14443_4() {
                                std::string poAid = "A000000291A000000191";

                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;
                                poApduRequestList = Arrays::asList(std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("9000"), true));

                                std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(poAid), nullptr), nullptr, nullptr);

                                std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(seSelector, poApduRequestList, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4);

                                return std::make_shared<SeRequestSet>(seRequest);

                            }

                            std::shared_ptr<SeRequestSet> SampleFactory::getASeRequestSet() {
                                std::string poAid = "A000000291A000000191";

                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;
                                poApduRequestList = Arrays::asList(std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("9000"), true));

                                std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(poApduRequestList, ChannelState::CLOSE_AFTER);

                                return std::make_shared<SeRequestSet>(seRequest);

                            }

                            std::shared_ptr<SeRequest> SampleFactory::getASeRequest_ISO14443_4() {
                                std::string poAid = "A000000291A000000191";

                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;
                                poApduRequestList = Arrays::asList(std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("9000"), true));

                                std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(poAid), nullptr), nullptr, nullptr);

                                std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(seSelector, poApduRequestList, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4);
                                return seRequest;

                            }

                            std::shared_ptr<SeRequest> SampleFactory::getASeRequest() {
                                std::string poAid = "A000000291A000000191";

                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;
                                poApduRequestList = Arrays::asList(std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("9000"), true));

                                std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(poApduRequestList, ChannelState::CLOSE_AFTER);
                                return seRequest;

                            }

                            std::shared_ptr<SeRequestSet> SampleFactory::getCompleteRequestSet() {
                                std::string poAid = "A000000291A000000191";

                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                                poApduRequestList = Arrays::asList(std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("9000"), true));

                                std::shared_ptr<SeSelector> aidSelector = std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(poAid), nullptr), nullptr, nullptr);


                                std::shared_ptr<SeSelector> atrSelector = std::make_shared<SeSelector>(nullptr, std::make_shared<SeSelector::AtrFilter>("/regex/"), nullptr);

                                std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(aidSelector, poApduRequestList, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4);

                                std::shared_ptr<SeRequest> seRequest2 = std::make_shared<SeRequest>(atrSelector, poApduRequestList, ChannelState::KEEP_OPEN, ContactsProtocols::PROTOCOL_ISO7816_3);

                                std::shared_ptr<Set<std::shared_ptr<SeRequest>>> seRequests = std::unordered_set<std::shared_ptr<SeRequest>>();
                                seRequests->add(seRequest);
                                seRequests->add(seRequest2);

                                return std::make_shared<SeRequestSet>(seRequests);


                            }

                            std::shared_ptr<SeResponseSet> SampleFactory::getCompleteResponseSet() {
                                std::vector<std::shared_ptr<SeResponse>> seResponses;

                                std::shared_ptr<ApduResponse> apdu = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000"), std::unordered_set<Integer>());
                                std::shared_ptr<ApduResponse> apdu2 = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000"), std::unordered_set<Integer>());

                                std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                apduResponses.push_back(apdu);
                                apduResponses.push_back(apdu2);

                                seResponses.push_back(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, apdu, true), apduResponses));
                                seResponses.push_back(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, apdu, true), apduResponses));

                                return std::make_shared<SeResponseSet>(seResponses);


                            }
                        }
                    }
                }
            }
        }
    }
}
