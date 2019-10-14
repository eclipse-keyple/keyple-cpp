#pragma once

#include "../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include <string>
#include <vector>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { class ReadRecordsRespPars; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class CalypsoPo; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace calypso {

                    using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                    using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                    using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;


                    class CommandSetTestSuite : public std::enable_shared_from_this<CommandSetTestSuite> {

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeAll public static void setUp()
                        static void setUp();

                    private:
                        static int getCounterValueFromByteArray(std::vector<char> &inByteArray, int inCounterIndex);


                        static std::shared_ptr<ReadRecordsRespPars> readRecords(std::shared_ptr<CalypsoPo> calypsoPo, Byte fileSfi, Byte recordNumber, ReadDataStructure readDataStructureValue) throw(KeypleReaderException);


                        static void updateRecord(std::shared_ptr<CalypsoPo> calypsoPo, Byte sfi, Byte recordNumber, std::vector<char> &dataToWrite) throw(KeypleReaderException);


                        static void decreaseCounter(std::shared_ptr<CalypsoPo> calypsoPo, Byte countersSfi, Byte counterIndex, int valueToDecrement) throw(KeypleReaderException);


                        static void increaseCounter(std::shared_ptr<CalypsoPo> calypsoPo, Byte countersSfi, Byte counterIndex, int valueToIncrement) throw(KeypleReaderException);


                        static void appendRecord(std::shared_ptr<CalypsoPo> calypsoPo, Byte sfi, std::vector<char> &dataToWrite) throw(KeypleReaderException);


                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testWriteCounter()
                        virtual void testWriteCounter();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDecreaseCounter()
                        virtual void testDecreaseCounter();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIncreaseCounter()
                        virtual void testIncreaseCounter();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testReadRecords()
                        virtual void testReadRecords();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAppendRecord()
                        virtual void testAppendRecord();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUpdateAndDecreaseCounterSingleSession()
                        virtual void testUpdateAndDecreaseCounterSingleSession();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUpdateAndIncreaseCounterSingleSession()
                        virtual void testUpdateAndIncreaseCounterSingleSession();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUpdateAndAppendSingleSession()
                        virtual void testUpdateAndAppendSingleSession();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testReadDataInOpenSession()
                        virtual void testReadDataInOpenSession();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testWriteDataBeforeProcessOpening()
                        virtual void testWriteDataBeforeProcessOpening();
                    };

                }
            }
        }
    }
}
