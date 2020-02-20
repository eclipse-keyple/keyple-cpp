#include "SeResponseTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SelectionStatus.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "ApduResponseTest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {
                    using org::junit::Assert;
                    using org::junit::Test;
                    using org::junit::runner::RunWith;
                    using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessfullResponseMatch() throws IllegalArgumentException
                    void SeResponseTest::constructorSuccessfullResponseMatch() throw(std::invalid_argument) {

                        std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), ApduResponseTest::getAFCI(), true), ApduResponseTest::getAListOfAPDUs());
                        Assert::assertNotNull(response);
                        Assert::assertArrayEquals(ApduResponseTest::getAListOfAPDUs().toArray(), response->getApduResponses().toArray());
                        Assert::assertEquals(true, response->wasChannelPreviouslyOpen());
                        Assert::assertEquals(ApduResponseTest::getAAtr(), response->getSelectionStatus()->getAtr());
                        Assert::assertEquals(ApduResponseTest::getAFCI(), response->getSelectionStatus()->getFci());
                        Assert::assertEquals(response->getSelectionStatus()->hasMatched(), true);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessfullResponseNoMatch() throws IllegalArgumentException
                    void SeResponseTest::constructorSuccessfullResponseNoMatch() throw(std::invalid_argument) {

                        std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), ApduResponseTest::getAFCI(), false), ApduResponseTest::getAListOfAPDUs());
                        Assert::assertNotNull(response);
                        Assert::assertArrayEquals(ApduResponseTest::getAListOfAPDUs().toArray(), response->getApduResponses().toArray());
                        Assert::assertEquals(true, response->wasChannelPreviouslyOpen());
                        Assert::assertEquals(ApduResponseTest::getAAtr(), response->getSelectionStatus()->getAtr());
                        Assert::assertEquals(ApduResponseTest::getAFCI(), response->getSelectionStatus()->getFci());
                        Assert::assertEquals(response->getSelectionStatus()->hasMatched(), false);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorATRNull() throws IllegalArgumentException
                    void SeResponseTest::constructorATRNull() throw(std::invalid_argument) {
                        std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(), true), ApduResponseTest::getAListOfAPDUs());
                        Assert::assertNotNull(response);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorFCINull() throws IllegalArgumentException
                    void SeResponseTest::constructorFCINull() throw(std::invalid_argument) {
                        std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), nullptr, true), ApduResponseTest::getAListOfAPDUs());
                        Assert::assertNotNull(response);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void constructorFCIAndATRNull() throws IllegalArgumentException
                    void SeResponseTest::constructorFCIAndATRNull() throw(std::invalid_argument) {
                        std::shared_ptr<SeResponse> response = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, nullptr, true), ApduResponseTest::getAListOfAPDUs());
                        Assert::assertNull(response);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testEquals() throws Exception
                    void SeResponseTest::testEquals() throw(std::runtime_error) {
                        Assert::assertTrue(getASeResponse()->equals(getASeResponse()));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testThisEquals() throws Exception
                    void SeResponseTest::testThisEquals() throw(std::runtime_error) {
                        std::shared_ptr<SeResponse> resp = getASeResponse();
                        Assert::assertTrue(resp->equals(resp));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testNotEquals() throws Exception
                    void SeResponseTest::testNotEquals() throw(std::runtime_error) {
                        std::shared_ptr<SeResponse> resp = getASeResponse();
                        std::shared_ptr<void> any;
                        Assert::assertFalse(resp->equals(any));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testNotEqualsNull() throws Exception
                    void SeResponseTest::testNotEqualsNull() throw(std::runtime_error) {
                        std::shared_ptr<SeResponse> resp = getASeResponse();
                        std::shared_ptr<SeResponse> respNull = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(), true), nullptr);
                        std::shared_ptr<SeResponse> respNull2 = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), nullptr, true), nullptr);
                        std::shared_ptr<SeResponse> respNull3 = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), ApduResponseTest::getAFCI(), true), nullptr);
                        Assert::assertFalse(resp->equals(respNull));
                        Assert::assertFalse(resp->equals(respNull2));
                        Assert::assertFalse(resp->equals(respNull3));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void hashcode() throws Exception
                    void SeResponseTest::hashcode() throw(std::runtime_error) {
                        std::shared_ptr<SeResponse> resp = getASeResponse();
                        std::shared_ptr<SeResponse> resp2 = getASeResponse();
                        Assert::assertTrue(resp->hashCode() == resp2->hashCode());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void hashcodeNull() throws Exception
                    void SeResponseTest::hashcodeNull() throw(std::runtime_error) {
                        std::shared_ptr<SeResponse> resp = std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, ApduResponseTest::getAFCI(), true), nullptr);
                        Assert::assertNotNull(resp->hashCode());
                    }

                    std::shared_ptr<SeResponse> SeResponseTest::getASeResponse() throw(std::invalid_argument) {
                        return std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(ApduResponseTest::getAAtr(), ApduResponseTest::getAFCI(), true), ApduResponseTest::getAListOfAPDUs());
                    }
                }
            }
        }
    }
}
