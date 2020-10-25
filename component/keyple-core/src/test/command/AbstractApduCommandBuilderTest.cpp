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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AbstractApduCommandBuilder.h"

#include "SeCommand.h"

using namespace keyple::core::command;

using namespace testing;

class SeCommandMock final : public SeCommand {
public:
    MOCK_METHOD(const std::string&,
                getName,
                (),
                (const, override));

    MOCK_METHOD(uint8_t,
                getInstructionByte,
                (),
                (const, override));
};

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder1)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractApduCommandBuilder builder(table, request);
}

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder1_getName)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractApduCommandBuilder builder(table, request);

    const std::string subName = "SubName";
    builder.addSubName(subName);

    ASSERT_EQ(builder.getName(), name + " - " + subName);
    ASSERT_EQ(builder.getApduRequest()->getName(), name + " - " + subName);
}

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder1_addSubName)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractApduCommandBuilder builder(table, request);

    ASSERT_EQ(builder.getName(), name);
}

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder1_getApduRequest)
{
    /* Instantiate AbstractApduCommandBuilder using first constructor */

    std::shared_ptr<SeCommandMock> table =
        std::make_shared<SeCommandMock>();

    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    const std::string name = "CommandsTableMock";
    EXPECT_CALL(*table.get(), getName()).WillRepeatedly(ReturnRef(name));

    AbstractApduCommandBuilder builder(table, request);

    ASSERT_EQ(builder.getApduRequest(), request);
}

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder2)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    const std::string name = "AbstractApduCommandBuilder2";
    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    AbstractApduCommandBuilder builder(name, request);
}

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder2_getName)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    const std::string name = "AbstractApduCommandBuilder2";
    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    AbstractApduCommandBuilder builder(name, request);

    ASSERT_EQ(builder.getName(), name);
}

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder2_addSubName)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    const std::string name = "AbstractApduCommandBuilder2";
    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    AbstractApduCommandBuilder builder(name, request);

    const std::string subName = "SubName";
    builder.addSubName(subName);

    ASSERT_EQ(builder.getName(), name + " - " + subName);
    ASSERT_EQ(builder.getApduRequest()->getName(), name + " - " + subName);
}

TEST(AbstractApduCommandBuilderTest, AbstractApduCommandBuilder2_getApduRequest)
{
    /* Instantiate AbstractApduCommandBuilder using second constructor */

    const std::string name = "AbstractApduCommandBuilder2";
    const std::vector<uint8_t> apdu = {0x11, 0x22, 0x33, 0X44};
    std::shared_ptr<ApduRequest> request =
        std::make_shared<ApduRequest>(apdu, false);

    AbstractApduCommandBuilder builder(name, request);

    ASSERT_EQ(builder.getApduRequest(), request);
}

