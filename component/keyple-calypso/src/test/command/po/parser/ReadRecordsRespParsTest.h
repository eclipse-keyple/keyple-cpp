#pragma once

#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

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
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {



//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class ReadRecordsRespParsTest
                            class ReadRecordsRespParsTest : public std::enable_shared_from_this<ReadRecordsRespParsTest> {
                            public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void readRecordRespPars_one_record()
                                virtual void readRecordRespPars_one_record();
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void readRecordRespPars_records()
                                virtual void readRecordRespPars_records();
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sampleMultipleRecordsParsing()
                                virtual void sampleMultipleRecordsParsing();
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void readRecordRespPars_one_record_sfi()
                                virtual void readRecordRespPars_one_record_sfi();
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void readRecordRespPars_records_sfi()
                                virtual void readRecordRespPars_records_sfi();
                            };

                        }
                    }
                }
            }
        }
