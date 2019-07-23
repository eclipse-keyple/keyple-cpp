#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "ChannelState.h"
#include "GenericSeSelectionRequest.h"
#include "KeypleBaseException.h"
#include "MatchingSelection.h"
#include "NoStackTraceThrowable.h"
#include "PcscPlugin.h"
#include "ReaderUtilities.h"
#include "SeCommonProtocols.h"
#include "SelectionsResult.h"
#include "SelectionStatus.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeResponse.h"
#include "SeSelection.h"
#include "SeSelector.h"

/* Examples */
#include "ReaderUtilities.h"

using namespace org::eclipse::keyple::common;
using namespace org::eclipse::keyple::core::selection;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::exception;
using namespace org::eclipse::keyple::core::seproxy::message;
using namespace org::eclipse::keyple::core::seproxy::protocol;
//using namespace org::eclipse::keyple::core::transaction;
using namespace org::eclipse::keyple::core::util;
using namespace org::eclipse::keyple::plugin::pcsc;
using namespace org::eclipse::keyple::example::generic::common;
using namespace org::eclipse::keyple::example::generic::pc;

class UseCase_Generic3_GroupedMultiSelection_Pcsc {

};

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(UseCase_Generic3_GroupedMultiSelection_Pcsc));

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the PC/SC plugin */
    PcscPlugin pcscplugin = PcscPlugin::getInstance();
    pcscplugin.initReaders();

    /* Assign PcscPlugin to the SeProxyService */
    SeProxyService& seProxyService = SeProxyService::getInstance();
    seProxyService.addPlugin(std::make_shared<PcscPlugin>(pcscplugin));
    std::shared_ptr<SeProxyService> shared_proxy = std::make_shared<SeProxyService>(seProxyService);

    /*
     * Get a SE reader ready to work with contactless SE. Use the getReader helper method from
     * the ReaderUtilities class.
     */
    std::shared_ptr<SeReader> seReader = ReaderUtilities::getDefaultContactLessSeReader(shared_proxy);

    /* Check if the reader exists */
    if (seReader == nullptr) {
        throw std::make_shared<IllegalStateException>("Bad SE reader setup");
    }

    logger->info("=============== UseCase Generic #3: AID based grouped explicit multiple selection ==================\n");
    logger->info("= SE Reader  NAME = %s\n", seReader->getName());

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        /* CLOSE_AFTER pour assurer la sélection de toutes les applications */
        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>();

        /* operate SE selection (change the AID here to adapt it to the SE used for the test) */
        //std::string seAidPrefix = "A000000404012509";
        std::string seAidPrefix = "304554502E494341";

        /* AID based selection (1st selection, later indexed 0 */
        {
        std::vector<char> aid = ByteArrayUtil::fromHex(seAidPrefix);
        std::shared_ptr<SeSelector::AidSelector::IsoAid> isoAid = std::make_shared<SeSelector::AidSelector::IsoAid>(aid);
        std::shared_ptr<SeSelector::AidSelector> aidSelector  = 
            std::make_shared<SeSelector::AidSelector>(isoAid, nullptr, SeSelector::AidSelector::FileOccurrence::FIRST,
                                                      SeSelector::AidSelector::FileControlInformation::FCI);
        std::shared_ptr<SeSelector> seSelector =
            std::make_shared<SeSelector>(SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr, aidSelector, "Initial selection #1");
        std::shared_ptr<GenericSeSelectionRequest> genericSeSelectionRequest =
            std::make_shared<GenericSeSelectionRequest>(seSelector, ChannelState::CLOSE_AFTER);
        seSelection->prepareSelection(genericSeSelectionRequest);
        }

        /* next selection (2nd selection, later indexed 1) */
        {
        std::vector<char> aid = ByteArrayUtil::fromHex(seAidPrefix);
        std::shared_ptr<SeSelector::AidSelector::IsoAid> isoAid = std::make_shared<SeSelector::AidSelector::IsoAid>(aid);
        std::shared_ptr<SeSelector::AidSelector> aidSelector  = 
            std::make_shared<SeSelector::AidSelector>(isoAid, nullptr, SeSelector::AidSelector::FileOccurrence::NEXT,
                                                      SeSelector::AidSelector::FileControlInformation::FCI);
        std::shared_ptr<SeSelector> seSelector =
            std::make_shared<SeSelector>(SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr, aidSelector, "Initial selection #2");
        std::shared_ptr<GenericSeSelectionRequest> genericSeSelectionRequest =
            std::make_shared<GenericSeSelectionRequest>(seSelector, ChannelState::CLOSE_AFTER);
        seSelection->prepareSelection(genericSeSelectionRequest);
        }
        /* next selection */
        {
        std::vector<char> aid = ByteArrayUtil::fromHex(seAidPrefix);
        std::shared_ptr<SeSelector::AidSelector::IsoAid> isoAid = std::make_shared<SeSelector::AidSelector::IsoAid>(aid);
        std::shared_ptr<SeSelector::AidSelector> aidSelector  = 
            std::make_shared<SeSelector::AidSelector>(isoAid, nullptr, SeSelector::AidSelector::FileOccurrence::NEXT,
                                                      SeSelector::AidSelector::FileControlInformation::FCI);
        std::shared_ptr<SeSelector> seSelector =
            std::make_shared<SeSelector>(SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr, aidSelector, "Initial selection #3");
        std::shared_ptr<GenericSeSelectionRequest> genericSeSelectionRequest =
            std::make_shared<GenericSeSelectionRequest>(seSelector, ChannelState::CLOSE_AFTER);
        seSelection->prepareSelection(genericSeSelectionRequest);
        }

        /*
         * Actual SE communication: operate through a single request the SE selection
         */
        std::shared_ptr<SelectionsResult> selectionsResult = seSelection->processExplicitSelection(seReader);
        if (selectionsResult->getMatchingSelections().size() > 0) {
            for (auto matchingSelection : selectionsResult->getMatchingSelections()) {
                std::shared_ptr<AbstractMatchingSe> matchingSe = matchingSelection->getMatchingSe();
                logger->info("Selection status for selection \"%s\" (indexed %d): \n\t\tATR: %s\n\t\tFCI: %s",
                             matchingSelection->getExtraInfo(), matchingSelection->getSelectionIndex(),
                             ByteArrayUtil::toHex(matchingSe->getSelectionStatus()->getAtr()->getBytes()),
                             ByteArrayUtil::toHex(matchingSe->getSelectionStatus()->getFci()->getDataOut()));
            }
        } else {
            logger->info("No SE matched the selection\n");
        }
    } else {
        logger->error("No SE were detected\n");
    }

    return 0;
}
