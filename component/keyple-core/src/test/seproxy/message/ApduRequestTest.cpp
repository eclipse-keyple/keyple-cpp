#include "ApduRequestTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
//                    import static org.junit.Assert.*;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::junit::Before;
                    using org::junit::Test;
                    using org::junit::runner::RunWith;
                    using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                    void ApduRequestTest::setUp() throw(std::runtime_error) {

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimpleAPDURequest()
                    void ApduRequestTest::testSimpleAPDURequest() {
                        std::shared_ptr<ApduRequest> request = std::make_shared<ApduRequest>(getACommand(), true);
                        assertNotNull(request);
                        assertEquals(nullptr, request->getName());
                        assertTrue(request->isCase4());
                        assertArrayEquals(getACommand(), request->getBytes());
                        assertEquals(nullptr, request->getSuccessfulStatusCodes());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals("ApduRequest: NAME = \"null\", RAWDATA = FEDCBA989005, case4", request->toString());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAPDURequest()
                    void ApduRequestTest::testAPDURequest() {
                        std::shared_ptr<ApduRequest> request = getApduSample();
                        assertNotNull(request);
                        assertTrue(request->isCase4());
                        assertArrayEquals(getACommand(), request->getBytes());
                        assertEquals(getAName(), request->getName());
                        assertEquals(getASuccessFulStatusCode(), request->getSuccessfulStatusCodes());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertEquals("ApduRequest: NAME = \"" + getAName() + "\", RAWDATA = FEDCBA989005, case4, additional successful status codes = 2328", request->toString());
                    }

                    std::shared_ptr<ApduRequest> ApduRequestTest::getApduSample() {
                        std::shared_ptr<std::set<Integer>> successfulStatusCodes = getASuccessFulStatusCode();
                        Boolean case4 = true;
                        std::vector<char> command = getACommand();
                        std::shared_ptr<ApduRequest> request = std::make_shared<ApduRequest>(command, case4, successfulStatusCodes);
                        request->setName(getAName());
                        return request;
                    }

                    std::vector<char> ApduRequestTest::getACommand() {
                        return ByteArrayUtils::fromHex("FEDCBA98 9005h");
                    }

                    std::shared_ptr<std::set<int>> ApduRequestTest::getASuccessFulStatusCode() {
                        std::shared_ptr<std::set<Integer>> successfulStatusCodes = std::unordered_set<Integer>();
                        successfulStatusCodes->add(std::stoi("9000"));
                        return successfulStatusCodes;
                    }

                    std::string ApduRequestTest::getAName() {
                        return "TEST";
                    }
                }
            }
        }
    }
}
