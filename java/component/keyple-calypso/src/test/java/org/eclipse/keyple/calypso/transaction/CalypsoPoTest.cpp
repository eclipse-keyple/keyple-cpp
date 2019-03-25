#include "CalypsoPoTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SelectionStatus.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/transaction/PoSelectionRequest.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/PoRevision.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                    using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SelectionStatus = org::eclipse::keyple::seproxy::message::SelectionStatus;
                    using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::junit::Assert;
                    using org::junit::Test;
                    using org::junit::runner::RunWith;
                    using org::mockito::junit::MockitoJUnitRunner;

                    std::shared_ptr<CalypsoPo> CalypsoPoTest::getPoApplicationByte(char applicationByte) {
                        std::shared_ptr<ApduResponse> fciData = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex(std::string::format(std::string("6F 22 84 08 315449432E494341") + "A5 16 BF0C 13 C7 08 0000000011223344" + "53 07 060A %02X 02200311 9000", applicationByte)), nullptr);
                        std::shared_ptr<SeResponse> selectionData = std::make_shared<SeResponse>(false, std::make_shared<SelectionStatus>(nullptr, fciData, true), nullptr);
                        std::shared_ptr<PoSelectionRequest> poSelectionRequest = std::make_shared<PoSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex("315449432E494341"), nullptr), nullptr, nullptr), ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoPo> calypsoPo = std::make_shared<CalypsoPo>(poSelectionRequest);
                        calypsoPo->setSelectionResponse(selectionData);
                        return calypsoPo;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void computePoRevision()
                    void CalypsoPoTest::computePoRevision() {
                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x01))->getRevision(), PoRevision::REV2_4);

                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x04))->getRevision(), PoRevision::REV2_4);

                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x06))->getRevision(), PoRevision::REV2_4);

                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x1F))->getRevision(), PoRevision::REV2_4);

                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x20))->getRevision(), PoRevision::REV3_1);

                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x27))->getRevision(), PoRevision::REV3_1);

                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x28))->getRevision(), PoRevision::REV3_2);

                        Assert::assertEquals(getPoApplicationByte(static_cast<char>(0x2F))->getRevision(), PoRevision::REV3_2);
                    }
                }
            }
        }
    }
}
