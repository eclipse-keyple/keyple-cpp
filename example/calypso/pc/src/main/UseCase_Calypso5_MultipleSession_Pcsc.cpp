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

#include "CalypsoClassicInfo.h"
#include "CalypsoUtilities.h"
#include "KeypleBaseException.h"
#include "ReaderUtilities.h"
#include "KeypleReaderNotFoundException.h"
#include "LoggerFactory.h"
#include "MatchingSelection.h"
#include "ObservableReader.h"
#include "PcscPlugin.h"
#include "PcscPluginFactory.h"
#include "PcscReader.h"
#include "PcscReadersSettings.h"
#include "PcscReadersSettings.h"
#include "PcscProtocolSetting.h"
#include "PcscReadersSettings.h"
#include "PoSelectionRequest.h"
#include "PoTransaction.h"
#include "ReaderEvent.h"
#include "SamResource.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeSelection.h"

/* Common */
#include "ByteArrayUtil.h"
#include "stringhelper.h"

using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::example::calypso::pc;
using namespace keyple::example::generic::pc;
using namespace keyple::plugin::pcsc;

class UseCase_Calypso5_MultipleSession_Pcsc {
};

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(UseCase_Calypso5_MultipleSession_Pcsc));

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign PcscPlugin to the SeProxyService */
    seProxyService.registerPlugin(new PcscPluginFactory());

    /*
     * Get a PO reader ready to work with Calypso PO. Use the getReader helper
     * method from the CalypsoUtilities class.
     */
    std::shared_ptr<SeReader> poReader = CalypsoUtilities::getDefaultPoReader();
    /*
     * Get a SAM reader ready to work with Calypso PO. Use the getReader helper
     * method from the CalypsoUtilities class.
     */
    std::shared_ptr<SamResource> samResource =
        CalypsoUtilities::getDefaultSamResource();

    /* Check if the readers exists */
    if (poReader == nullptr || samResource == nullptr) {
        throw std::make_shared<IllegalStateException>(
            "Bad PO or SAM reader setup");
    }

    logger->info("=============== UseCase Calypso #5: Po Authentication "
                 "==================\n");
    logger->info("= PO Reader  NAME = %s\n", poReader->getName().c_str());
    logger->info("= SAM Reader  NAME = %s\n",
                 samResource->getSeReader()->getName().c_str());

    /* Check if a PO is present in the reader */
    if (poReader->isSePresent()) {

        logger->info("========================================================="
                     "=========================\n");
        logger->info("= 1st PO exchange: AID based selection with reading of "
                     "Environment file.         =\n");
        logger->info("========================================================="
                     "=========================\n");

        /*
         * Prepare a Calypso PO selection
         */
        std::shared_ptr<SeSelection> seSelection =
            std::make_shared<SeSelection>();

        /*
         * Setting of an AID based selection of a Calypso REV3 PO
         *
         * Select the first application matching the selection AID whatever the
         * SE communication protocol keep the logical channel open after the
         * selection
         */

        /*
         * Calypso selection: configures a PoSelector with all the desired
         * attributes to make the selection and read additional information
         * afterwards
         */
        std::shared_ptr<PoSelectionRequest> poSelectionRequest =
            std::make_shared<PoSelectionRequest>(
                std::make_shared<PoSelector>(
                    SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
                    std::make_shared<PoSelector::PoAidSelector>(
                        std::make_shared<SeSelector::AidSelector::IsoAid>(
                            CalypsoClassicInfo::AID),
                        PoSelector::InvalidatedPo::REJECT),
                    StringHelper::formatSimple("AID: %s",
                                               CalypsoClassicInfo::AID)));

        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        seSelection->prepareSelection(poSelectionRequest);

        /*
         * Actual PO communication: operate through a single request the Calypso
         * PO selection and the file read
         */
        std::shared_ptr<SelectionsResult> selectionResult =
            seSelection->processExplicitSelection(poReader);

        if (selectionResult->hasActiveSelection()) {
            std::shared_ptr<MatchingSelection> matchingSelection =
                selectionResult->getActiveSelection();

            std::shared_ptr<CalypsoPo> calypsoPo =
                std::static_pointer_cast<CalypsoPo>(
                    matchingSelection->getMatchingSe());
            logger->info("The selection of the PO has succeeded\n");

            /* Go on with the reading of the first record of the EventLog file */
            logger->info("====================================================="
                         "=============================\n");
            logger->info("= 2nd PO exchange: open and close a secure session "
                         "to perform authentication.    =\n");
            logger->info("====================================================="
                         "=============================\n");

            std::shared_ptr<PoTransaction> poTransaction =
                std::make_shared<PoTransaction>(
                    std::make_shared<PoResource>(poReader, calypsoPo),
                    samResource, CalypsoUtilities::getSecuritySettings());

            /*
             * Open Session for the debit key
             */
            bool poProcessStatus = poTransaction->processOpening(
                PoTransaction::ModificationMode::MULTIPLE,
                PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, 0, 0);

            if (!poProcessStatus) {
                throw IllegalStateException("processingOpening failure.");
            }

            if (!poTransaction->wasRatified()) {
                logger->info("========= Previous Secure Session was not "
                             "ratified. =====================\n");
            }

            /*
             * Compute the number of append records (29 bytes) commands that
             * will overflow the PO modifications buffer. Each append records
             * will consume 35 (29 + 6) bytes in the buffer.
             *
             * We'll send one more command to demonstrate the MULTIPLE mode
             */
            int modificationsBufferSize = calypsoPo->getModificationsCounter();

            int nbCommands = (modificationsBufferSize / 35) + 1;

            std::vector<int> appendRecordParsers(nbCommands);

            logger->info(
                "==== Send %d Append Record commands. Modifications buffer "
                "capacity = %d bytes i.e. %d 29-byte commands ====\n",
                nbCommands, modificationsBufferSize,
                modificationsBufferSize / 35);

            for (int i = 0; i < nbCommands; i++) {
                std::vector<uint8_t> dataFill = ByteArrayUtil::fromHex(
                    CalypsoClassicInfo::eventLog_dataFill);
                appendRecordParsers[i] = poTransaction->prepareAppendRecordCmd(
                    CalypsoClassicInfo::SFI_EventLog, dataFill,
                    StringHelper::formatSimple("EventLog (SFI=%02X) #%d",
                                               CalypsoClassicInfo::SFI_EventLog,
                                               i));
            }

            /* proceed with the sending of commands, don't close the channel */
            poProcessStatus = poTransaction->processPoCommandsInSession();

            if (!poProcessStatus) {
                for (int i = 0; i < nbCommands; i++) {
                    if (!poTransaction
                             ->getResponseParser(appendRecordParsers[i])
                             ->isSuccessful()) {
                        logger->error(
                            "Append record #%d failed with error %s\n", i,
                            poTransaction
                                ->getResponseParser(appendRecordParsers[i])
                                ->getStatusInformation()
                                .c_str());
                    }
                }
            }

            /*
             * Close the Secure Session.
             */

            logger->info("========= PO Calypso session ======= Closing "
                         "============================\n");

            /*
             * A ratification command will be sent (CONTACTLESS_MODE).
             */
            poProcessStatus =
                poTransaction->processClosing(ChannelControl::KEEP_OPEN);

            if (!poProcessStatus) {
                throw new IllegalStateException("processClosing failure.");
            }

            logger->info("====================================================="
                         "=============================\n");
            logger->info("= End of the Calypso PO processing.                  "
                         "                            =\n");
            logger->info("====================================================="
                         "=============================\n");
        } else {
            logger->error("The selection of the PO has failed\n");
        }
    } else {
        logger->error("No PO were detected\n");
    }

    return 0;
}
