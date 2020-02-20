#include "ApduResponseTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/AnswerToReset.h"

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
//ORIGINAL LINE: @Before public void setUp()
                    void ApduResponseTest::setUp() {
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessFullResponse()
                    void ApduResponseTest::constructorSuccessFullResponse() {
                        std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("FEDCBA98 9000h"), nullptr);
                        assertNotNull(response);
                        assertEquals(0x9000, response->getStatusCode());
                        assertEquals("FEDCBA989000", ByteArrayUtils::toHex(response->getBytes()));
                        assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"), response->getDataOut());
                        assertTrue(response->isSuccessful());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessFullResponseWithCustomCode()
                    void ApduResponseTest::constructorSuccessFullResponseWithCustomCode() {
                        std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("FEDCBA98 9005h"), getA9005CustomCode());
                        assertNotNull(response);
                        assertEquals(0x9005, response->getStatusCode());
                        assertEquals("FEDCBA989005", ByteArrayUtils::toHex(response->getBytes()));
                        assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"), response->getDataOut());
                        assertTrue(response->isSuccessful());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorFailResponse()
                    void ApduResponseTest::constructorFailResponse() {
                        std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("FEDCBA98 9004h"), nullptr);
                        assertNotNull(response);
                        assertEquals("FEDCBA989004", ByteArrayUtils::toHex(response->getBytes()));
                        assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"), response->getDataOut());
                        assertEquals(0x9004, response->getStatusCode());
                        assertFalse(response->isSuccessful());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorFailResponseWithCustomCode()
                    void ApduResponseTest::constructorFailResponseWithCustomCode() {
                        std::shared_ptr<ApduResponse> response = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("FEDCBA98 9004h"), getA9005CustomCode());
                        assertNotNull(response);
                        assertEquals("FEDCBA989004", ByteArrayUtils::toHex(response->getBytes()));
                        assertArrayEquals(ByteArrayUtils::fromHex("FEDCBA98"), response->getDataOut());
                        assertEquals(0x9004, response->getStatusCode());
                        assertFalse(response->isSuccessful());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isEqualsTest()
                    void ApduResponseTest::isEqualsTest() {
                        assertTrue(getAFCI()->equals(getAFCI()));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isThisEquals()
                    void ApduResponseTest::isThisEquals() {
                        std::shared_ptr<ApduResponse> resp = getAFCI();
                        assertTrue(resp->equals(resp));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isNotEquals()
                    void ApduResponseTest::isNotEquals() {
                        std::shared_ptr<ApduResponse> resp = getAFCI();
                        std::shared_ptr<void> obj;
                        assertFalse(resp->equals(obj));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isNotEqualsNull()
                    void ApduResponseTest::isNotEqualsNull() {
                        std::shared_ptr<ApduResponse> resp = getAFCI();
                        std::shared_ptr<ApduResponse> respNull = std::make_shared<ApduResponse>(nullptr, nullptr);
                        assertFalse(resp->equals(respNull));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hashcodeTest()
                    void ApduResponseTest::hashcodeTest() {
                        std::shared_ptr<ApduResponse> resp = getAFCI();
                        std::shared_ptr<ApduResponse> resp2 = getAFCI();
                        assertTrue(resp->hashCode() == resp2->hashCode());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hashcodeNull()
                    void ApduResponseTest::hashcodeNull() {
                        std::shared_ptr<ApduResponse> resp = std::make_shared<ApduResponse>(nullptr, nullptr);
                        assertNotNull(resp->hashCode());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testToStringNull()
                    void ApduResponseTest::testToStringNull() {
                        std::shared_ptr<ApduResponse> resp = std::make_shared<ApduResponse>(nullptr, nullptr);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                        assertNotNull(resp->toString());
                    }

                    std::shared_ptr<std::set<Integer>> ApduResponseTest::getA9005CustomCode() {
                        std::shared_ptr<std::set<Integer>> successfulStatusCodes = std::unordered_set<Integer>();
                        successfulStatusCodes->add(0x9005);
                        return successfulStatusCodes;
                    }

                    std::shared_ptr<AnswerToReset> ApduResponseTest::getAAtr() {
                        return std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B8F8001804F0CA000000306030001000000006A"));
                    }

                    std::shared_ptr<ApduResponse> ApduResponseTest::getAFCI() {
                        return std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000"), nullptr);
                    }

                    std::shared_ptr<ApduResponse> ApduResponseTest::getSuccessfullResponse() {
                        return std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("FEDCBA98 9000h"), nullptr);
                    }

                    std::vector<std::shared_ptr<ApduResponse>> ApduResponseTest::getAListOfAPDUs() {
                        std::vector<std::shared_ptr<ApduResponse>> apdus;
                        apdus.push_back(getSuccessfullResponse());
                        return apdus;
                    }
                }
            }
        }
    }
}
