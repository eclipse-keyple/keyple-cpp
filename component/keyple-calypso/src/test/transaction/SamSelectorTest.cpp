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

#include "SamSelector.h"

/* Calypso */
#include "SamIdentifier.h"

using namespace testing;

using namespace keyple::calypso::transaction;

TEST(SamSelectorTest, test_AidSelectorNull)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()
                ->samIdentifier(SamIdentifier::builder()
                                    ->samRevision(SamRevision::S1D)
                                    .build())
                .build());

    ASSERT_EQ(samSelector->getAidSelector(), nullptr);
}

TEST(SamSelectorTest, test_SamRevision_S1D)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()
                ->samIdentifier(SamIdentifier::builder()
                                    ->samRevision(SamRevision::S1D)
                                    .build())
                .build());

    ASSERT_EQ("3B(.{6}|.{10})805A..80D?20.{4}.{8}829000",
              samSelector->getAtrFilter()->getAtrRegex());
}

TEST(SamSelectorTest, test_SamRevision_S1E)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()
                ->samIdentifier(SamIdentifier::builder()
                                    ->samRevision(SamRevision::S1E)
                                    .build())
                .build());

    ASSERT_EQ("3B(.{6}|.{10})805A..80E120.{4}.{8}829000",
              samSelector->getAtrFilter()->getAtrRegex());
}

TEST(SamSelectorTest, test_SamRevision_C1)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()
                ->samIdentifier(SamIdentifier::builder()
                                    ->samRevision(SamRevision::C1)
                                    .build())
                .build());

    ASSERT_EQ("3B(.{6}|.{10})805A..80C120.{4}.{8}829000",
              samSelector->getAtrFilter()->getAtrRegex());
}

TEST(SamSelectorTest, test_SamRevision_ANY)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()
                ->samIdentifier(SamIdentifier::builder()
                                    ->samRevision(SamRevision::AUTO)
                                    .build())
                .build());

    ASSERT_EQ(".*", samSelector->getAtrFilter()->getAtrRegex());
}

TEST(SamSelectorTest, test_SamSerialNumber)
{
    std::shared_ptr<SamSelector> samSelector =
        std::dynamic_pointer_cast<SamSelector>(
            SamSelector::builder()
                ->samIdentifier(SamIdentifier::builder()
                                    ->samRevision(SamRevision::C1)
                                    .serialNumber("11223344")
                                    .build())
                .build());

    ASSERT_EQ("3B(.{6}|.{10})805A..80C120.{4}11223344829000",
              samSelector->getAtrFilter()->getAtrRegex());
}
