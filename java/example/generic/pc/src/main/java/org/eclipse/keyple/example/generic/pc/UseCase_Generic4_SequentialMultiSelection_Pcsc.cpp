#include "ApduResponse.h"
#include "ByteArrayUtils.h"
#include "ChannelState.h"
#include "ContactlessProtocols.h"
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
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

using ApduResponse          = org::eclipse::keyple::seproxy::message::ApduResponse;
using ByteArrayUtils        = org::eclipse::keyple::util::ByteArrayUtils;
using ChannelState          = org::eclipse::keyple::seproxy::ChannelState;
using ContactlessProtocols  = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
using KeypleBaseException   = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
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

class UseCase_Generic4_SequentialMultiSelection_Pcsc {

};

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(UseCase_Generic4_SequentialMultiSelection_Pcsc));

void doAndAnalyseSelection(std::shared_ptr<SeSelection> seSelection,
                           std::shared_ptr<MatchingSe> matchingSe,
                           int index)
{
    if (seSelection->processExplicitSelection()) {
        logger->info("The SE matched the selection %d.", index);

        if (matchingSe->getSelectionSeResponse() != nullptr) {
            logger->info("Selection status for case %d: \n\t\tATR: " \
                         "%s\n\t\tFCI: %s", index,
                         ByteArrayUtils::toHex(matchingSe
                                               ->getSelectionSeResponse()
                                               ->getSelectionStatus()
                                               ->getAtr()->getBytes()),
                        ByteArrayUtils::toHex(matchingSe
                                              ->getSelectionSeResponse()
                                              ->getSelectionStatus()
                                              ->getFci()->getDataOut()));
        }
    }
    else {
        logger->info("The selection 2 process did not return any selected SE.");
    }
}

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

    logger->info("=============== UseCase Generic #4: AID based sequential explicit multiple selection ==================");
    logger->info("= SE Reader  NAME = %s", seReader->getName());

    std::shared_ptr<MatchingSe> matchingSe;

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(seReader);

        /* operate SE selection (change the AID here to adapt it to the SE used for the test) */
        std::string seAidPrefix = "A000000404012509";

        /* AID based selection */
        std::vector<char> aid = ByteArrayUtils::fromHex(seAidPrefix);
        std::shared_ptr<SeSelector::AidSelector> shared_aid = std::make_shared<SeSelector::AidSelector>(aid, nullptr, SeSelector::AidSelector::FileOccurrence::FIRST, SeSelector::AidSelector::FileControlInformation::FCI);
        matchingSe = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(shared_aid, nullptr, "Initial selection #1"), ChannelState::KEEP_OPEN, std::dynamic_pointer_cast<SeProtocol>(std::shared_ptr<ContactlessProtocols>(&ContactlessProtocols::PROTOCOL_ISO14443_4))));

        seSelection = std::make_shared<SeSelection>(seReader);

        doAndAnalyseSelection(seSelection, matchingSe, 1);

        /* next selection */
        matchingSe = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(shared_aid, nullptr, "Next selection #2"), ChannelState::KEEP_OPEN, std::dynamic_pointer_cast<SeProtocol>(std::shared_ptr<ContactlessProtocols>(&ContactlessProtocols::PROTOCOL_ISO14443_4))));

        seSelection = std::make_shared<SeSelection>(seReader);

        doAndAnalyseSelection(seSelection, matchingSe, 2);

        /* next selection */
        matchingSe = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(shared_aid, nullptr, "Next selection #3"), ChannelState::CLOSE_AFTER, std::dynamic_pointer_cast<SeProtocol>(std::shared_ptr<ContactlessProtocols>(&ContactlessProtocols::PROTOCOL_ISO14443_4))));

        doAndAnalyseSelection(seSelection, matchingSe, 3);

    }
    else {
        logger->error("No SE were detected.");
    }

    return 0;
}
