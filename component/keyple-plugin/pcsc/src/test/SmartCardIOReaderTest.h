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
#include "ResponseAPDU.h"

/* PC/SC plugin */
#include "PcscReader_Import.h"

using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
using Card           = org::eclipse::keyple::smartcardio::Card;
using CardChannel    = org::eclipse::keyple::smartcardio::CardChannel;
using CardTerminal   = org::eclipse::keyple::smartcardio::CardTerminal;
using PcscReader     = org::eclipse::keyple::plugin::pcsc::PcscReader;
using ResponseAPDU   = org::eclipse::keyple::smartcardio::ResponseAPDU;

class SmartCardIOReaderTest : public ::testing::Test {

public:
    PcscReader* reader;

    std::string readerName;

    std::vector<char> responseApduByte;

    CardTerminal* terminal;

    Card *card;

    CardChannel* channel;

    std::vector<char> atr;

    ResponseAPDU* res;

    SCARDCONTEXT ctx;

    void SetUp() //throw(CardException, std::invalid_argument, KeypleBaseException);
    {
        std::string s("dummy");
        terminal = new CardTerminal(ctx, s);
        if (!terminal)
            return;

        card = terminal->connect("");
        if (!card)
            return;

        channel = card->getBasicChannel();
        if (!channel)
            return;

        responseApduByte = ByteArrayUtils::fromHex("851700010000001212000001030101007E7E7E000000000000");
        res = new ResponseAPDU(responseApduByte);
        if (res == nullptr)
            return;

        readerName = "reader";
        std::shared_ptr<CardTerminal> shared_terminal = std::shared_ptr<CardTerminal>(terminal);
        reader = new PcscReader("pcscPlugin", shared_terminal);
        if (!reader)
            return;

        reader->setParameter(PcscReader::SETTING_KEY_LOGGING, "true");
    }

    void TearDown()
    {

    }
};
