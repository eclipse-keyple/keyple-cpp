#include "ApduResponse.h"
#include "ByteArrayUtils.h"
#include "ChannelState.h"
#include "ContactlessProtocols_Import.h"
#include "KeypleBaseException.h"
#include "MatchingSe.h"
#include "NoStackTraceThrowable.h"
#include "PcscPlugin.h"
#include "ReaderUtilities.h"
#include "SelectionStatus.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeResponse.h"
#include "SeSelector.h"
#include "SeSelection.h"
#include "SeSelectionRequest.h"

/* Examples */
#include "ReaderUtilities.h"

using ApduResponse          = org::eclipse::keyple::seproxy::message::ApduResponse;
using ByteArrayUtils        = org::eclipse::keyple::util::ByteArrayUtils;
using ChannelState          = org::eclipse::keyple::seproxy::ChannelState;
using ContactlessProtocols  = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
using KeypleBaseException   = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
using Logger                = org::eclipse::keyple::common::Logger;
using LoggerFactory         = org::eclipse::keyple::common::LoggerFactory;
using MatchingSe            = org::eclipse::keyple::transaction::MatchingSe;
using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
using PcscPlugin            = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
using ReaderUtilities       = org::eclipse::keyple::example::generic::pc::ReaderUtilities;
using SelectionStatus       = org::eclipse::keyple::seproxy::message::SelectionStatus;
using SeProtocol            = org::eclipse::keyple::seproxy::protocol::SeProtocol;
using SeProxyService        = org::eclipse::keyple::seproxy::SeProxyService;
using SeReader              = org::eclipse::keyple::seproxy::SeReader;
using SeResponse            = org::eclipse::keyple::seproxy::message::SeResponse;
using SeSelector            = org::eclipse::keyple::seproxy::SeSelector;
using SeSelection           = org::eclipse::keyple::transaction::SeSelection;
using SeSelectionRequest    = org::eclipse::keyple::transaction::SeSelectionRequest;

class UseCase_Generic1_ExplicitSelectionAid_Pcsc {

};

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(UseCase_Generic1_ExplicitSelectionAid_Pcsc));

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the PC/SC plugin */
    PcscPlugin pcscplugin = PcscPlugin::getInstance();
    pcscplugin.initReaders();
    std::shared_ptr<PcscPlugin> shared_plugin = std::shared_ptr<PcscPlugin>(&pcscplugin);

    /* Assign PcscPlugin to the SeProxyService */
    SeProxyService& seProxyService = SeProxyService::getInstance();
    seProxyService.addPlugin(shared_plugin);
    std::shared_ptr<SeProxyService> shared_proxy = std::shared_ptr<SeProxyService>(&seProxyService);

    /*
     * Get a SE reader ready to work with contactless SE. Use the getReader helper method from
     * the ReaderUtilities class.
     */
    std::shared_ptr<SeReader> seReader = ReaderUtilities::getDefaultContactLessSeReader(shared_proxy);

    /* Check if the reader exists */
    if (seReader == nullptr) {
        throw std::make_shared<IllegalStateException>("Bad SE reader setup");
    }

    logger->info("=============== UseCase Generic #3: AID based grouped explicit multiple selection ==================");
    logger->info("= SE Reader  NAME = %s", seReader->getName());

    std::vector<std::shared_ptr<MatchingSe>> matchingSeTable(3);

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        /* CLOSE_AFTER pour assurer la sélection de toutes les applications */
        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(seReader);

        /* operate SE selection (change the AID here to adapt it to the SE used for the test) */
        std::string seAidPrefix = "A000000404012509";

        /* AID based selection */
        std::vector<char> aid = ByteArrayUtils::fromHex(seAidPrefix);
        std::shared_ptr<SeSelector::AidSelector> shared_aid = std::make_shared<SeSelector::AidSelector>(aid, nullptr);
        matchingSeTable[0] = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(shared_aid, nullptr, "Initial selection #1"), ChannelState::CLOSE_AFTER, std::dynamic_pointer_cast<SeProtocol>(std::shared_ptr<ContactlessProtocols>(&ContactlessProtocols::PROTOCOL_ISO14443_4))));
        /* next selection */
        matchingSeTable[1] = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(shared_aid, nullptr, "Next selection #2"), ChannelState::CLOSE_AFTER, std::dynamic_pointer_cast<SeProtocol>(std::shared_ptr<ContactlessProtocols>(&ContactlessProtocols::PROTOCOL_ISO14443_4))));
        /* next selection */
        matchingSeTable[2] = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(shared_aid, nullptr, "Next selection #3"), ChannelState::CLOSE_AFTER, std::dynamic_pointer_cast<SeProtocol>(std::shared_ptr<ContactlessProtocols>(&ContactlessProtocols::PROTOCOL_ISO14443_4))));
        /*
            * Actual SE communication: operate through a single request the SE selection
            */
        if (seSelection->processExplicitSelection()) {

            int matchedSelection = 0;
            /* Count the number of SE that matched the selection */
            for (int i = 0; i < (int)matchingSeTable.size(); i++) {
                if (matchingSeTable[i]->getSelectionSeResponse() != nullptr) {
                    matchedSelection++;
                }
            }
            logger->info("The SE matched %d time(s) the selection.", matchedSelection);

            for (int i = 0; i < (int)matchingSeTable.size(); i++) {

                if (matchingSeTable[i]->getSelectionSeResponse() != nullptr) {
                    logger->info("Selection status for case %d: \n\t\tATR: %s\n\t\tFCI: %s", i + 1, ByteArrayUtils::toHex(matchingSeTable[i]->getSelectionSeResponse()->getSelectionStatus()->getAtr()->getBytes()), ByteArrayUtils::toHex(matchingSeTable[i]->getSelectionSeResponse()->getSelectionStatus()->getFci()->getDataOut()));
                }
            }
        }
        else {
            logger->info("The selection process did not return any selected SE.");
        }
    }
    else {
        logger->error("No SE were detected.");
    }

    return 0;
}
