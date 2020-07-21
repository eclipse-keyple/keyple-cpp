/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include <unordered_map>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractIso7816CommandBuilder.h"

#include "SeCommand.h"

using namespace keyple::core::command;

using namespace testing;

class SeCommandMock : public SeCommand {
public:
    MOCK_METHOD(const std::string&, getName, (), (const, override));
    MOCK_METHOD(uint8_t, getInstructionByte, (), (const, override));
    MOCK_METHOD(const std::type_info&, getCommandBuilderClass, (),
                (const, override));
    MOCK_METHOD(const std::type_info&, getResponseParserClass, (),
                (const, override));
};

class AbstractIso7816CommandBuilderMock
: public AbstractIso7816CommandBuilder {
public:
    AbstractIso7816CommandBuilderMock(
      std::shared_ptr<SeCommand> commandReference,
      std::shared_ptr<ApduRequest> request)
    : AbstractIso7816CommandBuilder(commandReference, request) {}

    AbstractIso7816CommandBuilderMock(const std::string& name,
                                      std::shared_ptr<ApduRequest> request)
    : AbstractIso7816CommandBuilder(name, request) {}

    std::shared_ptr<ApduRequest> setApduRequest(
        uint8_t cla, const std::shared_ptr<SeCommand> command, uint8_t p1,
        uint8_t p2, const std::vector<uint8_t>& dataIn) override
    {
        return AbstractIso7816CommandBuilder::setApduRequest(cla, command, p1,
                                                             p2, dataIn);
    }

    std::shared_ptr<ApduRequest> setApduRequest(
        uint8_t cla, const std::shared_ptr<SeCommand> command, uint8_t p1,
        uint8_t p2, const std::vector<uint8_t>& dataIn, uint8_t le) override
    {
        return AbstractIso7816CommandBuilder::setApduRequest(cla, command, p1,
	                                                     p2, dataIn, le);
    }

    std::shared_ptr<ApduRequest> setApduRequest(
        uint8_t cla, const std::shared_ptr<SeCommand> command, uint8_t p1,
        uint8_t p2, uint8_t le) override
    {
        return AbstractIso7816CommandBuilder::setApduRequest(cla, command, p1,
                                                             p2, le);
    }
};

TEST(AbstractIso7816CommandBuilderTest, AbstractIso7816CommandBuilder1)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44, 0x90, 0x00};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractIso7816CommandBuilder builder(table, request);
}

TEST(AbstractIso7816CommandBuilderTest,
     AbstractIso7816CommandBuilder1_setApduRequest_case2)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11};
    const std::vector<uint8_t> ref = {0xcc, 0x11, 0xb1, 0xb2, 0x06};

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractIso7816CommandBuilderMock builder(table, request);

    EXPECT_CALL(*table.get(), getInstructionByte())
        .WillRepeatedly(Return(static_cast < uint8_t>(0x11)));

    /* case 2: dataIn = null, le != null */
    std::shared_ptr<ApduRequest> req =
        builder.setApduRequest(0xcc, table, 0x0b1, 0x0b2, 0x06);

    ASSERT_EQ(req->getBytes(), ref);
}


TEST(AbstractIso7816CommandBuilderTest,
     AbstractIso7816CommandBuilder1_setApduRequest_case3)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0x44};
    const std::vector<uint8_t> ref = {0xcc, 0x11, 0xb1, 0xb2, 0x04, 0x11, 0x22,
                                      0x33, 0x44};

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractIso7816CommandBuilderMock builder(table, request);

    EXPECT_CALL(*table.get(), getInstructionByte())
        .WillRepeatedly(Return(static_cast<uint8_t>(0x11)));

    /* case 3: dataIn != null, le = null*/
    std::shared_ptr<ApduRequest> req =
        builder.setApduRequest(0xcc, table, 0xb1, 0xb2, apdu);

    ASSERT_EQ(req->getBytes(), ref);
}


TEST(AbstractIso7816CommandBuilderTest,
     AbstractIso7816CommandBuilder1_setApduRequest_case4)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0x44};
    const std::vector<uint8_t> ref = {0xcc, 0x11, 0xb1, 0xb2, 0x04, 0x11, 0x22,
                                      0x33, 0x44, 0x00};

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractIso7816CommandBuilderMock builder(table, request);

    EXPECT_CALL(*table.get(), getInstructionByte())
        .WillRepeatedly(Return(static_cast<uint8_t>(0x11)));

    /* case 4: dataIn = null, le = 0 */
    std::shared_ptr<ApduRequest> req =
        builder.setApduRequest(0xcc, table, 0xb1, 0xb2, apdu, 0x00);

    ASSERT_EQ(req->getBytes(), ref);
}

TEST(AbstractIso7816CommandBuilderTest, AbstractIso7816CommandBuilder2)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44, 0x90, 0x00};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);
    const std::string name = "AbstractIso7816CommandBuilder2";

    AbstractIso7816CommandBuilder builder(name, request);
}

TEST(AbstractIso7816CommandBuilderTest,
     AbstractIso7816CommandBuilder2_setApduRequest_case2)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11};
    const std::vector<uint8_t> ref = {0xcc, 0x11, 0xb1, 0xb2, 0x06};

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractIso7816CommandBuilderMock builder(name, request);

    EXPECT_CALL(*table.get(), getInstructionByte())
        .WillRepeatedly(Return(static_cast<uint8_t>(0x11)));

    /* case 2: dataIn = null, le != null */
    std::shared_ptr<ApduRequest> req =
        builder.setApduRequest(0xcc, table, 0x0b1, 0x0b2, 0x06);

    ASSERT_EQ(req->getBytes(), ref);
}


TEST(AbstractIso7816CommandBuilderTest,
     AbstractIso7816CommandBuilder2_setApduRequest_case3)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0x44};
    const std::vector<uint8_t> ref = {0xcc, 0x11, 0xb1, 0xb2, 0x04, 0x11, 0x22,
                                      0x33, 0x44};

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractIso7816CommandBuilderMock builder(name, request);

    EXPECT_CALL(*table.get(), getInstructionByte())
        .WillRepeatedly(Return(static_cast<uint8_t>(0x11)));

    /* case 3: dataIn != null, le = null*/
    std::shared_ptr<ApduRequest> req =
        builder.setApduRequest(0xcc, table, 0xb1, 0xb2, apdu);

    ASSERT_EQ(req->getBytes(), ref);
}


TEST(AbstractIso7816CommandBuilderTest,
     AbstractIso7816CommandBuilder2_setApduRequest_case4)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0x44};
    const std::vector<uint8_t> ref = {0xcc, 0x11, 0xb1, 0xb2, 0x04, 0x11, 0x22,
                                      0x33, 0x44, 0x00};

    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractIso7816CommandBuilderMock builder(name, request);

    EXPECT_CALL(*table.get(), getInstructionByte())
        .WillRepeatedly(Return(static_cast<uint8_t>(0x11)));

    /* case 4: dataIn = null, le = 0 */
    std::shared_ptr<ApduRequest> req =
        builder.setApduRequest(0xcc, table, 0xb1, 0xb2, apdu, 0x00);

    ASSERT_EQ(req->getBytes(), ref);
}
