#include "VirtualReaderTransmitTest.h"
#include "../../../../../../../../../../stub/src/test/java/org/eclipse/keyple/plugin/stub/StubReaderTest.h"
#include "../rm/json/SampleFactory.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../../../keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/builder/ReadRecordsCmdBuild.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace integration {
                        using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                        using ReadRecordsCmdBuild = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                        using SampleFactory = org::eclipse::keyple::plugin::remotese::rm::json::SampleFactory;
                        using StubReaderTest = org::eclipse::keyple::plugin::stub::StubReaderTest;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using namespace org::eclipse::keyple::seproxy::message;
                        using org::junit::After;
                        using org::junit::Assert;
                        using org::junit::Before;
                        using org::junit::Test;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualReaderTransmitTest::logger = org::slf4j::LoggerFactory::getLogger(VirtualReaderTransmitTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                        void VirtualReaderTransmitTest::setUp() throw(std::runtime_error) {
                            // restore plugin state
                            clearStubpluginReaders();

                            initKeypleServices();

                            // configure and connect a Stub Native reader
                            nativeReader = this->connectStubReader(NATIVE_READER_NAME, CLIENT_NODE_ID);

                            // test virtual reader
                            virtualReader = getVirtualReader();

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                        void VirtualReaderTransmitTest::tearDown() throw(std::runtime_error) {
                            clearStubpluginReaders();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testKOTransmitSet_NoSE()
                        void VirtualReaderTransmitTest::testKOTransmitSet_NoSE() {

                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmitSet(SampleFactory::getASeRequestSet());
                                // should throw KeypleReaderException
                                Assert::assertTrue(false);

                            }
                            catch (const KeypleReaderException &e) {
                                logger->info("KeypleReaderException was thrown as expected");
                                // assert exception is thrown
                                Assert::assertNotNull(e);
                                Assert::assertNotNull(e->getSeResponseSet());
                                Assert::assertNull(e->getSeResponse());
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testKOTransmit_NoSE()
                        void VirtualReaderTransmitTest::testKOTransmit_NoSE() {

                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmit(SampleFactory::getASeRequest());
                                // should throw KeypleReaderException
                                Assert::assertTrue(false);

                            }
                            catch (const KeypleReaderException &e) {
                                logger->info("KeypleReaderException was thrown as expected");
                                // assert exception is thrown
                                Assert::assertNotNull(e);
                                // Assert.assertNotNull(e.getSeResponseSet());
                                // Assert.assertNull(e.getSeResponse());
                                // should not be null but transmit is using transmitSet, this is the reason I guess
                                // todo : VirtualReader transmit should not be using transmitSet
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void rse_transmit_Hoplink_Sucessfull() throws Exception
                        void VirtualReaderTransmitTest::rse_transmit_Hoplink_Sucessfull() throw(std::runtime_error) {
                            int N_TIMES = 10;

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::hoplinkSE());

                            delay(1000);

                            StubReaderTest::selectSe(virtualReader);

                            // test N_TIMES transmit with KEEP_OPEN
                            for (int i = 0; i < N_TIMES; i++) {

                                // test
                                std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "");
                                std::vector<std::shared_ptr<ApduRequest>> poApduRequestList = Arrays::asList(poReadRecordCmd_T2Env->getApduRequest());
                                std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(poApduRequestList, ChannelState::KEEP_OPEN);
                                std::shared_ptr<SeResponseSet> seResponse = virtualReader->transmitSet(std::make_shared<SeRequestSet>(seRequest));
                                // assert
                                Assert::assertTrue(seResponse->getSingleResponse()->getApduResponses()[0]->isSuccessful());

                                logger->info("SeResponseSet returned as expected {}", seResponse->getSingleResponse());
                            }


                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void rse_transmit_no_response() throws Exception
                        void VirtualReaderTransmitTest::rse_transmit_no_response() throw(std::runtime_error) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::noApduResponseSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequestSet> requests = StubReaderTest::getNoResponseRequest();

                            StubReaderTest::selectSe(virtualReader);

                            // test
                            virtualReader->transmitSet(requests);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_0() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_set_0() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequestSet> seRequestSet = StubReaderTest::getPartialRequestSet(0);

                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmitSet(seRequestSet);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : {} {}", ex->getSeResponseSet(), ex->getSeResponse());

                                Assert::assertEquals(ex->getSeResponseSet()->getResponses().size(), 1);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[0]->getApduResponses()->size(), 2);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_1() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_set_1() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequestSet> seRequestSet = StubReaderTest::getPartialRequestSet(1);

                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmitSet(seRequestSet);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : {} {}", ex->getSeResponseSet(), ex->getSeResponse());
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses().size(), 2);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[0]->getApduResponses()->size(), 4);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[1]->getApduResponses()->size(), 2);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[1]->getApduResponses()->size(), 2);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_2() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_set_2() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequestSet> seRequestSet = StubReaderTest::getPartialRequestSet(2);

                            // test
                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmitSet(seRequestSet);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : {} {}", ex->getSeResponseSet(), ex->getSeResponse());
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses().size(), 3);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[0]->getApduResponses()->size(), 4);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[1]->getApduResponses()->size(), 4);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[2]->getApduResponses()->size(), 2);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_3() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_set_3() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequestSet> seRequestSet = StubReaderTest::getPartialRequestSet(3);

                            // test
                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmitSet(seRequestSet);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : {} {}", ex->getSeResponseSet(), ex->getSeResponse());
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses().size(), 3);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[0]->getApduResponses()->size(), 4);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[1]->getApduResponses()->size(), 4);
                                Assert::assertEquals(ex->getSeResponseSet()->getResponses()[2]->getApduResponses()->size(), 4);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_0() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_0() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequest> seRequest = StubReaderTest::getPartialRequest(0);

                            // test
                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmit(seRequest);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : {} {}", ex->getSeResponseSet(), ex->getSeResponse());
                                Assert::assertEquals(ex->getSeResponse()->getApduResponses().size(), 0);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_1() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_1() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequest> seRequest = StubReaderTest::getPartialRequest(1);

                            // test
                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmit(seRequest);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : {} {}", ex->getSeResponseSet(), ex->getSeResponse());
                                Assert::assertEquals(ex->getSeResponse()->getApduResponses().size(), 1);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_2() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_2() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequest> seRequest = StubReaderTest::getPartialRequest(2);

                            // test
                            try {
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmit(seRequest);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : set : {}, seResponse : {}", ex->getSeResponseSet(), ex->getSeResponse());
                                Assert::assertEquals(ex->getSeResponse()->getApduResponses().size(), 2);
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_3() throws InterruptedException
                        void VirtualReaderTransmitTest::transmit_partial_response_3() throw(InterruptedException) {

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::partialSE());

                            // wait for card to be detected
                            delay(500);

                            // init Request
                            std::shared_ptr<SeRequest> seRequest = StubReaderTest::getPartialRequest(3);

                            try {
                                // test
                                StubReaderTest::selectSe(virtualReader);

                                virtualReader->transmit(seRequest);

                            }
                            catch (const KeypleReaderException &ex) {
                                logger->info("KeypleReaderException was thrown as expected : {} {}", ex->getSeResponseSet(), ex->getSeResponse());
                                Assert::assertEquals(ex->getSeResponse()->getApduResponses().size(), 3);
                            }
                        }
                    }
                }
            }
        }
    }
}
