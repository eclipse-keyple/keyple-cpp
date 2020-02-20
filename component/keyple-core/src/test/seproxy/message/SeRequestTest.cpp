#include "SeRequestTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "ApduRequestTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
//                    import static org.junit.Assert.*;
                    using ChannelState = keyple::core::seproxy::ChannelState;
                    using SeSelector = keyple::core::seproxy::SeSelector;
                    using ContactlessProtocols = keyple::core::seproxy::protocol::ContactlessProtocols;
                    using Protocol = keyple::core::seproxy::protocol::Protocol;
                    using SeProtocol = keyple::core::seproxy::protocol::SeProtocol;
                    using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

                    std::vector<std::shared_ptr<ApduRequest>> SeRequestTest::getApdus() {
                        return apdus;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
                    void SeRequestTest::setUp() {

                        apdus = getAapduLists();
                        channelState = ChannelState::KEEP_OPEN;
                        seProtocol = getASeProtocol();
                        selectionStatusCode = ApduRequestTest::getASuccessFulStatusCode();
                        selector = getSelector(selectionStatusCode);
                        seRequest = std::make_shared<SeRequest>(selector, apdus, channelState, seProtocol);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSERequest()
                    void SeRequestTest::testSERequest() {
                        assertNotNull(seRequest);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getSelector()
                    void SeRequestTest::getSelector() {
                        // test
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getSelector(selectionStatusCode)->toString(), seRequest->getSeSelector()->toString());

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getApduRequests()
                    void SeRequestTest::getApduRequests() {
                        // test
                        seRequest = std::make_shared<SeRequest>(getSelector(nullptr), apdus, ChannelState::CLOSE_AFTER, Protocol::ANY);
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isKeepChannelOpen()
                    void SeRequestTest::isKeepChannelOpen() {
                        assertTrue(seRequest->isKeepChannelOpen());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getProtocolFlag()
                    void SeRequestTest::getProtocolFlag() {
                        seRequest = std::make_shared<SeRequest>(getSelector(nullptr), std::vector<std::shared_ptr<ApduRequest>>(), ChannelState::KEEP_OPEN, seProtocol);
                        assertEquals(seProtocol, seRequest->getProtocolFlag());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getSuccessfulSelectionStatusCodes()
                    void SeRequestTest::getSuccessfulSelectionStatusCodes() {
                        seRequest = std::make_shared<SeRequest>(getSelector(selectionStatusCode), std::vector<std::shared_ptr<ApduRequest>>(), ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_B_PRIME);
                        assertArrayEquals(selectionStatusCode->toArray(), seRequest->getSeSelector()->getAidSelector()->getSuccessfulSelectionStatusCodes()->toArray());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void toStringNull()
                    void SeRequestTest::toStringNull() {
                        seRequest = std::make_shared<SeRequest>(nullptr, nullptr, nullptr, nullptr);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertNotNull(seRequest->toString());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor1()
                    void SeRequestTest::constructor1() {
                        seRequest = std::make_shared<SeRequest>(getSelector(nullptr), apdus, channelState, Protocol::ANY);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getSelector(nullptr)->toString(), seRequest->getSeSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        //
                        assertEquals(Protocol::ANY, seRequest->getProtocolFlag());
                        assertNull(seRequest->getSeSelector()->getAidSelector()->getSuccessfulSelectionStatusCodes());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor2()
                    void SeRequestTest::constructor2() {
                        seRequest = std::make_shared<SeRequest>(getSelector(nullptr), apdus, channelState, seProtocol);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getSelector(nullptr)->toString(), seRequest->getSeSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        assertEquals(seProtocol, seRequest->getProtocolFlag());
                        //
                        assertNull(seRequest->getSeSelector()->getAidSelector()->getSuccessfulSelectionStatusCodes());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor2b()
                    void SeRequestTest::constructor2b() {
                        seRequest = std::make_shared<SeRequest>(getSelector(selectionStatusCode), apdus, channelState, Protocol::ANY);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getSelector(selectionStatusCode)->toString(), seRequest->getSeSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        assertEquals(Protocol::ANY, seRequest->getProtocolFlag());
                        //
                        assertArrayEquals(selectionStatusCode->toArray(), seRequest->getSeSelector()->getAidSelector()->getSuccessfulSelectionStatusCodes()->toArray());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor3()
                    void SeRequestTest::constructor3() {
                        seRequest = std::make_shared<SeRequest>(getSelector(selectionStatusCode), apdus, channelState, seProtocol);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getSelector(selectionStatusCode)->toString(), seRequest->getSeSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        assertEquals(seProtocol, seRequest->getProtocolFlag());
                        assertArrayEquals(selectionStatusCode->toArray(), seRequest->getSeSelector()->getAidSelector()->getSuccessfulSelectionStatusCodes()->toArray());
                    }

                    std::shared_ptr<SeRequest> SeRequestTest::getSeRequestSample() {

                        std::vector<std::shared_ptr<ApduRequest>> apdus = getAapduLists();
                        ChannelState channelState = ChannelState::KEEP_OPEN;
                        std::shared_ptr<SeProtocol> seProtocol = getASeProtocol();
                        std::shared_ptr<std::set<Integer>> selectionStatusCode = ApduRequestTest::getASuccessFulStatusCode();

                        return std::make_shared<SeRequest>(getSelector(selectionStatusCode), apdus, channelState, seProtocol);

                    }

                    std::vector<std::shared_ptr<ApduRequest>> SeRequestTest::getAapduLists() {
                        std::vector<std::shared_ptr<ApduRequest>> apdus;
                        apdus = std::vector<std::shared_ptr<ApduRequest>>();
                        apdus.push_back(ApduRequestTest::getApduSample());
                        apdus.push_back(ApduRequestTest::getApduSample());
                        return apdus;
                    }

                    std::shared_ptr<SeProtocol> SeRequestTest::getASeProtocol() {
                        return ContactlessProtocols::PROTOCOL_B_PRIME;
                    }

                    std::shared_ptr<SeSelector> SeRequestTest::getSelector(std::shared_ptr<std::set<Integer>> selectionStatusCode) {
                        /*
                         * We can use a fake AID here because it is not fully interpreted, the purpose of this unit
                         * test is to verify the proper format of the request.
                         */
                        std::shared_ptr<SeSelector::AidSelector> aidSelector = std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex("AABBCCDDEEFF"), selectionStatusCode);
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(aidSelector, nullptr, nullptr);
                        return seSelector;
                    }
                }
            }
        }
    }
}
