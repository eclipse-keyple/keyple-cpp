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

#include <fstream>

#include "CalypsoClassicInfo.h"
#include "CalypsoSam.h"
#include "CalypsoUtilities.h"
#include "ChannelState.h"
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "MatchingSelection.h"
#include "ReaderUtilities.h"
#include "SamResource.h"
#include "SamSelector.h"
#include "SamSelectionRequest.h"
#include "SecuritySettings.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeSelection.h"
#include "SeSelector_Import.h"

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::generic::pc;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;

std::shared_ptr<Properties> CalypsoUtilities::properties;

CalypsoUtilities::StaticConstructor::StaticConstructor()
{
    properties = std::make_shared<Properties>();

    std::string propertiesFileName =
        "/Volumes/macbook-air-extension/git/cna/"
        "github.com.calypsonet.keyple-cpp/build/bin/config.properties";

    std::ifstream inputStream;
    inputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        inputStream.open(propertiesFileName, std::ifstream::in);
    } catch (int errorCode) {
    } catch (std::ifstream::failure& e) {
    }

    try {
        properties->load(inputStream);
    } catch (const FileNotFoundException& e) {
    } catch (const IOException& e) {
    }
}

CalypsoUtilities::StaticConstructor CalypsoUtilities::staticConstructor;

std::shared_ptr<SeReader> CalypsoUtilities::getDefaultPoReader()
{
    std::shared_ptr<SeReader> poReader = ReaderUtilities::getReaderByName(
        properties->getProperty("po.reader.regex"));

    ReaderUtilities::setContactlessSettings(poReader);

    return poReader;
}

std::shared_ptr<SamResource> CalypsoUtilities::getDefaultSamResource()
{
    std::shared_ptr<SeReader> samReader = ReaderUtilities::getReaderByName(
        properties->getProperty("sam.reader.regex"));

    ReaderUtilities::setContactsSettings(samReader);

    /*
     * Open logical channel for the SAM inserted in the reader
     *
     * (We expect the right is inserted)
     */
    try {
        return checkSamAndOpenChannel(samReader);
    } catch (IllegalStateException& e) {
        throw;
    }
}

std::shared_ptr<SecuritySettings> CalypsoUtilities::getSecuritySettings()
{
    /* define the security parameters to provide when creating PoTransaction */
    return std::make_shared<SecuritySettings>();
}

std::shared_ptr<SamResource>
CalypsoUtilities::checkSamAndOpenChannel(std::shared_ptr<SeReader> samReader)
{
    /*
     * check the availability of the SAM doing a ATR based selection, open its physical and
     * logical channels and keep it open
     */
    std::shared_ptr<SeSelection> samSelection = std::make_shared<SeSelection>();
    std::shared_ptr<SamSelector> samSelector  = std::make_shared<SamSelector>(
        SamRevision::C1, ".*", "Selection SAM C1");

    /* Prepare selector, ignore MatchingSe here */
    samSelection->prepareSelection(std::make_shared<SamSelectionRequest>(
        samSelector, ChannelState::KEEP_OPEN));
    std::shared_ptr<CalypsoSam> calypsoSam = nullptr;

    try {
        std::shared_ptr<SelectionsResult> selectionResult =
            samSelection->processExplicitSelection(samReader);
        if (!selectionResult) {
            throw IllegalStateException(
                "Unable to open a logical channel for SAM!");
        }

        std::shared_ptr<MatchingSelection> matchingSelection =
            selectionResult->getActiveSelection();
        if (!matchingSelection) {
            throw IllegalStateException(
                "Unable to open a logical channel for SAM!");
        }

        std::shared_ptr<AbstractMatchingSe> amse =
            matchingSelection->getMatchingSe();
        if (!amse) {
            throw IllegalStateException(
                "Unable to open a logical channel for SAM!");
        }

        calypsoSam = std::dynamic_pointer_cast<CalypsoSam>(amse);
        if (!calypsoSam || !calypsoSam->isSelected()) {
            throw IllegalStateException(
                "Unable to open a logical channel for SAM!");
        }
    } catch (const KeypleReaderException& e) {
        throw IllegalStateException(
            StringHelper::formatSimple("Reader exception: %s", e.what()));
    }

    return std::make_shared<SamResource>(samReader, calypsoSam);
}

}
}
}
}
}
