#include "SeRequestTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
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
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                    using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::junit::Before;
                    using org::junit::Test;
                    using org::junit::runner::RunWith;
                    using org::mockito::junit::MockitoJUnitRunner;

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
                        selector = getAidSelector();
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), apdus, channelState, seProtocol, selectionStatusCode);
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
                        assertEquals(getAidSelector()->toString(), seRequest->getSelector()->toString());

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getApduRequests()
                    void SeRequestTest::getApduRequests() {
                        // test
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), apdus, ChannelState::CLOSE_AFTER, Protocol::ANY, nullptr);
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
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), std::vector<std::shared_ptr<ApduRequest>>(), ChannelState::KEEP_OPEN, seProtocol, nullptr);
                        assertEquals(seProtocol, seRequest->getProtocolFlag());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getSuccessfulSelectionStatusCodes()
                    void SeRequestTest::getSuccessfulSelectionStatusCodes() {
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), std::vector<std::shared_ptr<ApduRequest>>(), ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_B_PRIME, selectionStatusCode);
                        assertArrayEquals(selectionStatusCode->toArray(), seRequest->getSuccessfulSelectionStatusCodes()->toArray());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void toStringNull()
                    void SeRequestTest::toStringNull() {
                        seRequest = std::make_shared<SeRequest>(nullptr, nullptr, ChannelState::KEEP_OPEN, Protocol::ANY, nullptr);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertNotNull(seRequest->toString());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor1()
                    void SeRequestTest::constructor1() {
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), apdus, channelState, Protocol::ANY, nullptr);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getAidSelector()->toString(), seRequest->getSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        //
                        assertEquals(Protocol::ANY, seRequest->getProtocolFlag());
                        assertNull(seRequest->getSuccessfulSelectionStatusCodes());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor2()
                    void SeRequestTest::constructor2() {
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), apdus, channelState, seProtocol, nullptr);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getAidSelector()->toString(), seRequest->getSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        assertEquals(seProtocol, seRequest->getProtocolFlag());
                        //
                        assertNull(seRequest->getSuccessfulSelectionStatusCodes());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor2b()
                    void SeRequestTest::constructor2b() {
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), apdus, channelState, Protocol::ANY, selectionStatusCode);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getAidSelector()->toString(), seRequest->getSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        assertEquals(Protocol::ANY, seRequest->getProtocolFlag());
                        //
                        assertArrayEquals(selectionStatusCode->toArray(), seRequest->getSuccessfulSelectionStatusCodes()->toArray());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructor3()
                    void SeRequestTest::constructor3() {
                        seRequest = std::make_shared<SeRequest>(getAidSelector(), apdus, channelState, seProtocol, selectionStatusCode);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals(getAidSelector()->toString(), seRequest->getSelector()->toString());
                        assertEquals(channelState == ChannelState::KEEP_OPEN, seRequest->isKeepChannelOpen());
                        assertArrayEquals(apdus.toArray(), seRequest->getApduRequests().toArray());
                        assertEquals(seProtocol, seRequest->getProtocolFlag());
                        assertArrayEquals(selectionStatusCode->toArray(), seRequest->getSuccessfulSelectionStatusCodes()->toArray());
                    }

                    std::shared_ptr<SeRequest> SeRequestTest::getSeRequestSample() {

                        std::vector<std::shared_ptr<ApduRequest>> apdus = getAapduLists();
                        ChannelState channelState = ChannelState::KEEP_OPEN;
                        std::shared_ptr<SeProtocol> seProtocol = getASeProtocol();
                        std::shared_ptr<Set<Integer>> selectionStatusCode = ApduRequestTest::getASuccessFulStatusCode();

                        return std::make_shared<SeRequest>(getAidSelector(), apdus, channelState, seProtocol, selectionStatusCode);

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

                    std::shared_ptr<SeRequest::Selector> SeRequestTest::getAidSelector() {
                        return std::make_shared<SeRequest::AidSelector>(ByteArrayUtils::fromHex("A000000291A000000191"));
                    }
                }
            }
        }
    }
}
