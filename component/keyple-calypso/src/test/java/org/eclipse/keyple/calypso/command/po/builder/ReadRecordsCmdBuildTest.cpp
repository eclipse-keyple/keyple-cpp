#include "ReadRecordsCmdBuildTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/builder/ReadRecordsCmdBuild.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                            using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                            using org::junit::Assert;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void readRecords_rev2_4() throws IllegalArgumentException
                            void ReadRecordsCmdBuildTest::readRecords_rev2_4() throw(std::invalid_argument) {

                                char cla = static_cast<char>(0x94);
                                char cmd = static_cast<char>(0xB2);
                                bool readJustOneRecord = false;
                                char sfi = static_cast<char>(0x08);
                                char p2 = static_cast<char>(static_cast<char>(sfi * 8) + 5);

                                // revision 2.4
                                std::vector<char> request2_4 = {cla, cmd, record_number, p2, 0x00};
                                apduCommandBuilder = std::make_shared<ReadRecordsCmdBuild>(PoClass::LEGACY, sfi, record_number, readJustOneRecord, expectedLength, "TestRev2_4");
                                apduRequest = apduCommandBuilder->getApduRequest();
                                Assert::assertArrayEquals(request2_4, apduRequest->getBytes());
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void readRecords_rev3_1() throws IllegalArgumentException
                            void ReadRecordsCmdBuildTest::readRecords_rev3_1() throw(std::invalid_argument) {

                                char cla = static_cast<char>(0x00);
                                char cmd = static_cast<char>(0xB2);
                                bool readJustOneRecord = false;
                                char sfi = static_cast<char>(0x08);
                                char p2 = static_cast<char>(static_cast<char>(sfi * 8) + 5);


                                // revision 3.1
                                std::vector<char> request3_1 = {cla, cmd, record_number, p2, 0x00};
                                apduCommandBuilder = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, sfi, record_number, readJustOneRecord, expectedLength, "TestRev3_1");
                                apduRequest = apduCommandBuilder->getApduRequest();
                                Assert::assertArrayEquals(request3_1, apduRequest->getBytes());
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void readRecords_rev3_2() throws IllegalArgumentException
                            void ReadRecordsCmdBuildTest::readRecords_rev3_2() throw(std::invalid_argument) {
                                char cla = static_cast<char>(0x00);
                                char cmd = static_cast<char>(0xB2);
                                bool readJustOneRecord = false;
                                char sfi = static_cast<char>(0x08);
                                char p2 = static_cast<char>(static_cast<char>(sfi * 8) + 5);

                                // revision 3.2
                                std::vector<char> request3_2 = {cla, cmd, record_number, p2, 0x00};
                                apduCommandBuilder = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, sfi, record_number, readJustOneRecord, expectedLength, "TestRev3_2");
                                apduRequest = apduCommandBuilder->getApduRequest();
                                Assert::assertArrayEquals(request3_2, apduRequest->getBytes());
                            }
                        }
                    }
                }
            }
        }
    }
}
