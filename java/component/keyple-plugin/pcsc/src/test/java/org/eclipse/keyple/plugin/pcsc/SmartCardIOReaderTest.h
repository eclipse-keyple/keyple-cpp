/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association
 * https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information
 * regarding copyright ownership.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ******************************************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

#include "gtest/gtest.h"

/* Smartcard I/O */
#include "Card.h"
#include "CardChannel.h"
#include "CardTerminal.h"

/* Core */
#include "ResponseAPDU.h"

/* PC/SC plugin */
#include "PcscReader_Import.h"

using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
using PcscReader = org::eclipse::keyple::plugin::pcsc::PcscReader;

class SmartCardIOReaderTest : public ::testing::Test {

public:
    std::shared_ptr<PcscReader> reader;

    std::string readerName;

    std::vector<char> responseApduByte;

    std::shared_ptr<CardTerminal> terminal;

    std::shared_ptr<Card> card;

    std::shared_ptr<CardChannel> channel;

    std::vector<char> atr;

    std::shared_ptr<ResponseAPDU> res;

    void SetUp() //throw(CardException, std::invalid_argument, KeypleBaseException);
    {
        std::string s("dummy");
        terminal = std::shared_ptr<CardTerminal>(new CardTerminal(s));
        if (terminal == nullptr)
            return;

        card = terminal->connect("");
        if (card == nullptr)
            return;

        channel = card->getBasicChannel();
        if (channel == nullptr)
            return;

        responseApduByte = ByteArrayUtils::fromHex("851700010000001212000001030101007E7E7E000000000000");
        res = std::shared_ptr<ResponseAPDU>(new ResponseAPDU(responseApduByte));
        if (res == nullptr)
            return;

        readerName = "reader";
        reader = std::shared_ptr<PcscReader>(new PcscReader("pcscPlugin", terminal));
        if (reader == nullptr)
            return;

        reader->setParameter(PcscReader::SETTING_KEY_LOGGING, "true");
    }

    void TearDown()
    {

    }
};
