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

#include "SamSelectorTest.h"
#include "SamSelector.h"
#include "ByteArrayUtil.h"

namespace keyple {
namespace calypso {
namespace transaction {
using SamRevision    = keyple::calypso::command::sam::SamRevision;
using SamSelector    = keyple::calypso::transaction::SamSelector;
using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

void SamSelectorTest::test_AidSelectorNull()
{
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(SamRevision::S1D, nullptr, "S1D");
    ASSERT_EQ(samSelector->getAidSelector(), nullptr);
}

void SamSelectorTest::test_SamRevision_S1D()
{
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(SamRevision::S1D, nullptr, "S1D");
    ASSERT_EQ("3B(.{6}|.{10})805A..80D?20.{4}.{8}829000",
              samSelector->getAtrFilter()->getAtrRegex());
}

void SamSelectorTest::test_SamRevision_S1E()
{
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(SamRevision::S1E, nullptr, "S1E");
    ASSERT_EQ("3B(.{6}|.{10})805A..80E120.{4}.{8}829000",
              samSelector->getAtrFilter()->getAtrRegex());
}

void SamSelectorTest::test_SamRevision_C1()
{
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(SamRevision::C1, nullptr, "C1");
    ASSERT_EQ("3B(.{6}|.{10})805A..80C120.{4}.{8}829000",
              samSelector->getAtrFilter()->getAtrRegex());
}

void SamSelectorTest::test_SamRevision_ANY()
{
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(SamRevision::AUTO, nullptr, "Any");
    ASSERT_EQ(".*", samSelector->getAtrFilter()->getAtrRegex());
}

void SamSelectorTest::test_SamSerialNumber()
{
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(SamRevision::C1, "11223344", "Any");
    ASSERT_EQ("3B(.{6}|.{10})805A..80C120.{4}11223344829000",
              samSelector->getAtrFilter()->getAtrRegex());
}

void SamSelectorTest::test_ExtraInfo()
{
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(SamRevision::AUTO, nullptr, "Any");
    ASSERT_EQ("Any", samSelector->getExtraInfo());
}
}
}
}
