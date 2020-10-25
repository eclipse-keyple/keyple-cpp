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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "StubPoolPlugin.h"

/* Common */
#include "stringhelper.h"

/* Core */
#include "KeypleAllocationNoReaderException.h"

/* Plugin */
#include "StubPoolPluginImpl.h"
#include "StubPoolPluginFactory.h"
#include "StubSecureElement.h"

using namespace testing;

using namespace keyple::common;
using namespace keyple::plugin::stub;

static const std::string POOL_PLUGIN_NAME = "pool1";

/**
 * Stub Secure Element
 */
class SPP_StubSecureElement_Mock final : public StubSecureElement {
public:
    const std::vector<uint8_t>& getATR() override
    {
        return mATR;
    }

    std::string getSeProcotol() override
    {
        return "";
    }

private:
    std::vector<uint8_t> mATR;
};

std::shared_ptr<SPP_StubSecureElement_Mock> stubSe =
    std::make_shared<SPP_StubSecureElement_Mock>();

/* Plug a pool reader */
TEST(StubPoolPluginTest, plugStubPoolReader_success)
{
    StubPoolPluginFactory factory(POOL_PLUGIN_NAME);
    auto stubPoolPlugin = std::dynamic_pointer_cast<StubPoolPluginImpl>(factory.getPlugin());

    std::shared_ptr<SeReader> seReader =
        stubPoolPlugin->plugStubPoolReader(
            "anyGroup",
            "anyName",
            std::dynamic_pointer_cast<StubSecureElement>(stubSe));

    ASSERT_EQ(static_cast<int>(stubPoolPlugin->getReaders().size()), 1);
    ASSERT_TRUE(seReader->isSePresent());
    ASSERT_EQ(static_cast<int>(stubPoolPlugin->getReaderGroupReferences().size()), 1);
}

/* Unplug a pool reader */
TEST(StubPoolPluginTest, unplugStubPoolReader_success)
{
    StubPoolPluginFactory factory(POOL_PLUGIN_NAME);
    auto stubPoolPlugin = std::dynamic_pointer_cast<StubPoolPluginImpl>(factory.getPlugin());

    /* Plug a reader */
    stubPoolPlugin->plugStubPoolReader("anyGroup", "anyName", stubSe);

    /* uplug the reader */
    stubPoolPlugin->unplugStubPoolReader("anyGroup");

    ASSERT_EQ(static_cast<int>(stubPoolPlugin->getReaders().size()), 0);
    ASSERT_EQ(static_cast<int>(stubPoolPlugin->getReaderGroupReferences().size()), 0);
}

/* Allocate one reader */
TEST(StubPoolPluginTest, allocate_success)
{
    /* Init stubPoolPlugin */
    StubPoolPluginFactory factory(POOL_PLUGIN_NAME);
    auto stubPoolPlugin = std::dynamic_pointer_cast<StubPoolPluginImpl>(factory.getPlugin());

    /* Plug readers */
    stubPoolPlugin->plugStubPoolReader("group1", "stub1", stubSe);
    stubPoolPlugin->plugStubPoolReader("group2", "stub2", stubSe);

    /* Allocate Reader */
    std::shared_ptr<SeReader> seReader = stubPoolPlugin->allocateReader("group1");

    /* Check allocate result is correct */
    ASSERT_TRUE(StringHelper::startsWith(seReader->getName(), "stub1"));

    /* Check allocate list is correct */
    const std::map<const std::string, const std::string>& readers =
        stubPoolPlugin->listAllocatedReaders();

    ASSERT_TRUE(readers.find("stub1") != readers.end());
    ASSERT_EQ(static_cast<int>(readers.size()), 1);
}

/* Allocate twice the same reader */
TEST(StubPoolPluginTest, allocate_twice)
{
    /* Init stubPoolPlugin */
    StubPoolPluginFactory factory(POOL_PLUGIN_NAME);
    auto stubPoolPlugin = std::dynamic_pointer_cast<StubPoolPluginImpl>(factory.getPlugin());

    /* Plug readers */
    stubPoolPlugin->plugStubPoolReader("group1", "stub1", stubSe);
    stubPoolPlugin->plugStubPoolReader("group2", "stub2", stubSe);

    /* Allocate Reader */
    std::shared_ptr<SeReader> seReader = stubPoolPlugin->allocateReader("group1");

    EXPECT_THROW(stubPoolPlugin->allocateReader("group1"),
                 KeypleAllocationNoReaderException);

}

/* Release one reader */
TEST(StubPoolPluginTest, release_success)
{
    /* Init stubPoolPlugin */
    StubPoolPluginFactory factory(POOL_PLUGIN_NAME);
    auto stubPoolPlugin = std::dynamic_pointer_cast<StubPoolPluginImpl>(factory.getPlugin());

    /* Plug readers */
    stubPoolPlugin->plugStubPoolReader("group1", "stub1", stubSe);
    stubPoolPlugin->plugStubPoolReader("group2", "stub2", stubSe);

    /* Allocate Reader */
    std::shared_ptr<SeReader> seReader = stubPoolPlugin->allocateReader("group1");

    /* Release reader */
    stubPoolPlugin->releaseReader(seReader);

    /* Assert no reader is allocated */
    ASSERT_EQ(static_cast<int>(stubPoolPlugin->listAllocatedReaders().size()), 0);
}
