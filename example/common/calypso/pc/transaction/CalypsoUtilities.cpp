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

#include "CalypsoUtilities.h"

#include <fstream>

#include "CalypsoClassicInfo.h"
#include "ChannelControl.h"
#include "KeypleReaderException.h"
#include "SamSelector.h"
#include "SamSelectionRequest.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeSelection.h"
#include "SeSelector.h"

/* Calypso */
#include "CalypsoSam.h"
#include "PoSecuritySettings.h"

/* Common */
#include "FileNotFoundException.h"
#include "IllegalStateException.h"
#include "IOException.h"

/* Core */
#include "SeCommonProtocols.h"

/* Examples */
#include "ReaderUtilities.h"

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace keyple::calypso::transaction;
using namespace keyple::common::exception;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::example::common;
using namespace keyple::example::calypso::common::postructure;

std::shared_ptr<Properties> CalypsoUtilities::properties;
const std::shared_ptr<Logger> CalypsoUtilities::mLogger =
    LoggerFactory::getLogger(typeid(CalypsoUtilities));

CalypsoUtilities::CalypsoUtilities() {}

CalypsoUtilities::StaticConstructor::StaticConstructor()
{
    properties = std::make_shared<Properties>();

    std::string propertiesFileName = "config.properties";

    std::ifstream inputStream;
    inputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        inputStream.open(propertiesFileName, std::ifstream::in);
    } catch (int errorCode) {
        (void)errorCode;
    } catch (std::ifstream::failure& e) {
        (void)e;
    }

    try {
        properties->load(inputStream);
    } catch (const FileNotFoundException& e) {
        mLogger->error("File not found exception: %\n", e.getMessage());
    } catch (const IOException& e) {
        mLogger->error("IO exception: %\n", e.getMessage());
    }
}

CalypsoUtilities::StaticConstructor CalypsoUtilities::staticConstructor;

std::shared_ptr<SeReader> CalypsoUtilities::getDefaultPoReader()
{
    std::shared_ptr<SeReader> poReader =
        ReaderUtilities::getReaderByName(properties->getProperty("po.reader.regex"));

    ReaderUtilities::setContactlessSettings(poReader);

    return poReader;
}

std::shared_ptr<SeResource<CalypsoSam>> CalypsoUtilities::getDefaultSamResource()
{
    std::shared_ptr<SeReader> samReader =
        ReaderUtilities::getReaderByName(properties->getProperty("sam.reader.regex"));

    ReaderUtilities::setContactsSettings(samReader);

    /*
     * Open logical channel for the SAM inserted in the reader
     *
     * (We expect the right is inserted)
     */
    return checkSamAndOpenChannel(samReader);
}

std::shared_ptr<PoSecuritySettings> CalypsoUtilities::getSecuritySettings(
    std::shared_ptr<SeResource<CalypsoSam>> samResource)
{
    /* The default KIF values for personalization, loading and debiting */
    const uint8_t DEFAULT_KIF_PERSO = 0x21;
    const uint8_t DEFAULT_KIF_LOAD = 0x27;
    const uint8_t DEFAULT_KIF_DEBIT = 0x30;

    /*
     * The default key record number values for personalization, loading and debiting
     * The actual value should be adjusted.
     */
    const uint8_t DEFAULT_KEY_RECORD_NUMBER_PERSO = 0x01;
    const uint8_t DEFAULT_KEY_RECORD_NUMBER_LOAD = 0x02;
    const uint8_t DEFAULT_KEY_RECORD_NUMBER_DEBIT = 0x03;

    /* Define the security parameters to provide when creating PoTransaction */
    return PoSecuritySettings::PoSecuritySettingsBuilder(samResource)
               .sessionDefaultKif(AccessLevel::SESSION_LVL_PERSO, DEFAULT_KIF_PERSO)
               .sessionDefaultKif(AccessLevel::SESSION_LVL_LOAD, DEFAULT_KIF_LOAD)
               .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
               .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_PERSO,
                                              DEFAULT_KEY_RECORD_NUMBER_PERSO)
               .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_LOAD,
                                              DEFAULT_KEY_RECORD_NUMBER_LOAD)
               .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                              DEFAULT_KEY_RECORD_NUMBER_DEBIT)
               .build();
}

std::shared_ptr<SeResource<CalypsoSam>> CalypsoUtilities::checkSamAndOpenChannel(
    std::shared_ptr<SeReader> samReader)
{
    /*
     * check the availability of the SAM doing a ATR based selection, open its physical and
     * logical channels and keep it open
     */
    auto samSelection = std::make_shared<SeSelection>();
    std::shared_ptr<SeSelector> seSelector =
        SamSelector::builder()->samRevision(SamRevision::C1)
                               .serialNumber(".*")
                               .build();
    auto samSelector = std::dynamic_pointer_cast<SamSelector>(seSelector);
    auto request = std::make_shared<SamSelectionRequest>(samSelector);
    auto abstract = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(request);

    /* Prepare selector, ignore MatchingSe here */
    samSelection->prepareSelection(abstract);
    std::shared_ptr<CalypsoSam> calypsoSam = nullptr;

    try {
        if (samReader->isSePresent()) {
            std::shared_ptr<SelectionsResult> selectionsResult =
                samSelection->processExplicitSelection(samReader);

            if (selectionsResult->hasActiveSelection()) {
                calypsoSam =
                    std::dynamic_pointer_cast<CalypsoSam>(selectionsResult->getActiveMatchingSe());
            } else {
                throw IllegalStateException("Unable to open a logical channel for SAM!");
            }
        } else {
            throw IllegalStateException("No SAM is present in the reader " + samReader->getName());
        }
    } catch (const KeypleReaderException& e) {
        throw IllegalStateException("Reader exception: " + std::string(e.what()));
    } catch (const KeypleException& e) {
        throw IllegalStateException("Reader exception: " + e.getMessage());
    }

    mLogger->info("The SAM resource has been created\n");

    return std::make_shared<SeResource<CalypsoSam>>(samReader, calypsoSam);
}

}
}
}
}
}
