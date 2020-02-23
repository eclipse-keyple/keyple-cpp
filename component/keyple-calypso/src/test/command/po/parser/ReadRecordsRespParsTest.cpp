/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "ReadRecordsRespParsTest.h"

using namespace keyple::calypso::command::po::parser;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

void ReadRecordsRespParsTest::readRecordRespPars_one_record()
{
}

void ReadRecordsRespParsTest::readRecordRespPars_records()
{
}

void ReadRecordsRespParsTest::sampleMultipleRecordsParsing()
{
}

void ReadRecordsRespParsTest::readRecordRespPars_one_record_sfi()
{
}

void ReadRecordsRespParsTest::readRecordRespPars_records_sfi()
{
}
}
}
}
}
}

TEST(ReadRecordsRespParsTest, testA)
{
    std::shared_ptr<ReadRecordsRespParsTest> LocalTest =
        std::make_shared<ReadRecordsRespParsTest>();
    LocalTest->readRecordRespPars_one_record();
}

TEST(ReadRecordsRespParsTest, testB)
{
    std::shared_ptr<ReadRecordsRespParsTest> LocalTest =
        std::make_shared<ReadRecordsRespParsTest>();
    LocalTest->readRecordRespPars_records();
}

TEST(ReadRecordsRespParsTest, testC)
{
    std::shared_ptr<ReadRecordsRespParsTest> LocalTest =
        std::make_shared<ReadRecordsRespParsTest>();
    LocalTest->sampleMultipleRecordsParsing();
}

TEST(ReadRecordsRespParsTest, testD)
{
    std::shared_ptr<ReadRecordsRespParsTest> LocalTest =
        std::make_shared<ReadRecordsRespParsTest>();
    LocalTest->readRecordRespPars_one_record_sfi();
}

TEST(ReadRecordsRespParsTest, testE)
{
    std::shared_ptr<ReadRecordsRespParsTest> LocalTest =
        std::make_shared<ReadRecordsRespParsTest>();
    LocalTest->readRecordRespPars_records_sfi();
}
