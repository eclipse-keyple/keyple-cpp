/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "PoSecuritySettings.h"

/* Calypso */
#include "CalypsoPo.h"
#include "ElementaryFile.h"
#include "FileData.h"
#include "PoSelector.h"
#include "SelectFileControl.h"

/* Core */
#include "ByteArrayUtil.h"
#include "SeCommonProtocols.h"

using namespace testing;

using namespace keyple::calypso;
using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

using AtrFilter = PoSelector::AtrFilter;
using FileType = FileHeader::FileType;
using InvalidatedPo = PoSelector::InvalidatedPo;
using PoSecuritySettingsBuilder = PoSecuritySettings::PoSecuritySettingsBuilder;

static const std::string ATR1 = "3B001122805A0180D002030411223344829000";
/* The default KIF values for personalization, loading and debiting */
static const uint8_t DEFAULT_KIF_PERSO = 0x21;
static const uint8_t DEFAULT_KIF_LOAD = 0x27;
static const uint8_t DEFAULT_KIF_DEBIT = 0x30;
static const uint8_t DEFAULT_KVC_PERSO = 0x11;
static const uint8_t DEFAULT_KVC_LOAD = 0x22;
static const uint8_t DEFAULT_KVC_DEBIT = 0x33;
/*
 * The default key record number values for personalization, loading and debiting
 * The actual value should be adjusted.
 */
static const uint8_t DEFAULT_KEY_RECORD_NUMBER_PERSO = 0x01;
static const uint8_t DEFAULT_KEY_RECORD_NUMBER_LOAD = 0x02;
static const uint8_t DEFAULT_KEY_RECORD_NUMBER_DEBIT = 0x03;

static std::shared_ptr<CalypsoSam> createCalypsoSam()
{
    auto atr = std::make_shared<AnswerToReset>(ByteArrayUtil::fromHex(ATR1));
    auto selectionStatus = std::make_shared<SelectionStatus>(atr, nullptr, true);
    auto response = std::make_shared<SeResponse>(true, true, selectionStatus, nullptr);
    return std::make_shared<CalypsoSam>(response, TransmissionMode::CONTACTS);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_nominal)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings =
        PoSecuritySettingsBuilder(samResource)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_PERSO, DEFAULT_KIF_PERSO)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_LOAD, DEFAULT_KIF_LOAD)
            .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
            .sessionDefaultKvc(AccessLevel::SESSION_LVL_PERSO, DEFAULT_KVC_PERSO)
            .sessionDefaultKvc(AccessLevel::SESSION_LVL_LOAD, DEFAULT_KVC_LOAD)
            .sessionDefaultKvc(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KVC_DEBIT)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_PERSO,
                DEFAULT_KEY_RECORD_NUMBER_PERSO)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_LOAD,
                 DEFAULT_KEY_RECORD_NUMBER_LOAD)
            .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                DEFAULT_KEY_RECORD_NUMBER_DEBIT)
            .build();

    ASSERT_EQ(poSecuritySettings->getSessionDefaultKif(AccessLevel::SESSION_LVL_PERSO),
              DEFAULT_KIF_PERSO);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKif(AccessLevel::SESSION_LVL_LOAD),
              DEFAULT_KIF_LOAD);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT),
              DEFAULT_KIF_DEBIT);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKvc(AccessLevel::SESSION_LVL_PERSO),
              DEFAULT_KVC_PERSO);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKvc(AccessLevel::SESSION_LVL_LOAD),
              DEFAULT_KVC_LOAD);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKvc(AccessLevel::SESSION_LVL_DEBIT),
              DEFAULT_KVC_DEBIT);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_PERSO),
              DEFAULT_KEY_RECORD_NUMBER_PERSO);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_LOAD),
              DEFAULT_KEY_RECORD_NUMBER_LOAD);
    ASSERT_EQ(poSecuritySettings->getSessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT),
              DEFAULT_KEY_RECORD_NUMBER_DEBIT);
    ASSERT_EQ(poSecuritySettings->getSamResource(), samResource);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_modificationMode_default)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource).build();

    ASSERT_EQ(poSecuritySettings->getSessionModificationMode(), ModificationMode::ATOMIC);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_modificationMode_Atomic)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                                  .sessionModificationMode(ModificationMode::ATOMIC)
                                  .build();

    ASSERT_EQ(poSecuritySettings->getSessionModificationMode(), ModificationMode::ATOMIC);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_modificationMode_Multiple)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                                  .sessionModificationMode(ModificationMode::MULTIPLE)
                                  .build();

    ASSERT_EQ(poSecuritySettings->getSessionModificationMode(),  ModificationMode::MULTIPLE);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_ratificationMode_default)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource).build();

    ASSERT_EQ(poSecuritySettings->getRatificationMode(), RatificationMode::CLOSE_RATIFIED);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_ratificationMode_CloseRatified)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                                  .ratificationMode(RatificationMode::CLOSE_RATIFIED)
                                  .build();

    ASSERT_EQ(poSecuritySettings->getRatificationMode(), RatificationMode::CLOSE_RATIFIED);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_ratificationMode_CloseNotRatified)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                                  .ratificationMode(RatificationMode::CLOSE_NOT_RATIFIED)
                                  .build();

    ASSERT_EQ(poSecuritySettings->getRatificationMode(), RatificationMode::CLOSE_NOT_RATIFIED);
}


TEST(PoSecuritySettingsTest, poSecuritySettings_pinTransmissionMode_default)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource).build();

    ASSERT_EQ(poSecuritySettings->getPinTransmissionMode(), PinTransmissionMode::ENCRYPTED);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_pinTransmissionMode_plain)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                                  .pinTransmissionMode(PinTransmissionMode::PLAIN)
                                  .build();

    ASSERT_EQ(poSecuritySettings->getPinTransmissionMode(), PinTransmissionMode::PLAIN);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_pinTransmissionMode_encrypted)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                                  .pinTransmissionMode(PinTransmissionMode::ENCRYPTED)
                                  .build();

    ASSERT_EQ(poSecuritySettings->getPinTransmissionMode(), PinTransmissionMode::ENCRYPTED);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_authorizedKvcList)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    const std::vector<uint8_t> authorizedKvcs = {0x12, 0x34};
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                                  .sessionAuthorizedKvcList(authorizedKvcs)
                                  .build();

    ASSERT_TRUE(poSecuritySettings->isSessionKvcAuthorized(0x12));
    ASSERT_TRUE(poSecuritySettings->isSessionKvcAuthorized(0x34));
    ASSERT_FALSE(poSecuritySettings->isSessionKvcAuthorized(0x56));
}

TEST(PoSecuritySettingsTest, poSecuritySettings_defaultPinCipheringKey)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings = PoSecuritySettingsBuilder(samResource).pinCipheringKey(0x11, 0x22)
                                                                    .build();
    std::shared_ptr<KeyReference> keyReference = poSecuritySettings->getDefaultPinCipheringKey();

    ASSERT_EQ(keyReference->getKif(), 0x11);
    ASSERT_EQ(keyReference->getKvc(), 0x22);
}

TEST(PoSecuritySettingsTest, poSecuritySettings_negativeSvBalance)
{
    std::shared_ptr<SeReader> samReader =  nullptr;
    std::shared_ptr<CalypsoSam> calypsoSam =  createCalypsoSam();
    auto samResource = std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
    auto poSecuritySettings =
            PoSecuritySettingsBuilder(samResource)
                    .build();
    ASSERT_EQ(poSecuritySettings->getSvNegativeBalance(), NegativeBalance::FORBIDDEN);

    poSecuritySettings = PoSecuritySettingsBuilder(samResource)
                             .svNegativeBalance(NegativeBalance::AUTHORIZED)
                             .build();

    ASSERT_EQ(poSecuritySettings->getSvNegativeBalance(), NegativeBalance::AUTHORIZED);
}
