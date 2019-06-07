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
#include "SeSelection.h"
#include "SeSelectionRequest.h"
#include "SeSelector.h"

using ByteArrayUtils        = org::eclipse::keyple::util::ByteArrayUtils;
using ChannelState          = org::eclipse::keyple::seproxy::ChannelState;
using ContactlessProtocols  = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
using KeypleBaseException   = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
using Logger                = org::eclipse::keyple::common::Logger;
using MatchingSe            = org::eclipse::keyple::transaction::MatchingSe;
using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
using PcscPlugin            = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
using ReaderUtilities       = org::eclipse::keyple::example::generic::pc::ReaderUtilities;
using SelectionStatus       = org::eclipse::keyple::seproxy::message::SelectionStatus;
using SeProtocol            = org::eclipse::keyple::seproxy::protocol::SeProtocol;
using SeProxyService        = org::eclipse::keyple::seproxy::SeProxyService;
using SeReader              = org::eclipse::keyple::seproxy::SeReader;
using SeSelector            = org::eclipse::keyple::seproxy::SeSelector;
using SeSelection           = org::eclipse::keyple::transaction::SeSelection;
using SeSelectionRequest    = org::eclipse::keyple::transaction::SeSelectionRequest;

//static std::string seAid = "A0000004040125090101";
static std::string seAid = "A000000291";
//static std::string seAid = "A00000019102";

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the PC/SC plugin */
    PcscPlugin pcscplugin = PcscPlugin::getInstance();
    pcscplugin.initReaders();

    /* Get the instance of the SeProxyService (Singleton pattern) and assign PcscPlugin to the SeProxyService */
    SeProxyService& seProxyService = SeProxyService::getInstance();
    seProxyService.addPlugin(std::make_shared<PcscPlugin>(pcscplugin));
    std::shared_ptr<SeProxyService> shared_seProxyService = std::make_shared<SeProxyService>(seProxyService);

    /*
     * Get a SE reader ready to work with generic SE. Use the getReader helper method from the
     * ReaderUtilities class.
     */
    std::shared_ptr<SeReader> seReader = ReaderUtilities::getDefaultContactLessSeReader(shared_seProxyService);

    /* Check if the reader exists */
    if (seReader == nullptr) {
        throw IllegalStateException("Bad SE reader setup");
    }

    std::cout << "=============== UseCase Generic #1: AID based explicit selection ==================" << std::endl;
    std::cout << "= SE Reader  NAME = " << seReader->getName() << std::endl;

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        std::cout << "==================================================================================" << std::endl;
        std::cout << "= AID based selection.                                                           =" << std::endl;
        std::cout << "==================================================================================" << std::endl;

        /*
         * Prepare the SE selection
         */
        SeSelection* seSelection = new SeSelection(seReader);

        /*
            * Setting of an AID based selection (in this example a Calypso REV3 PO)
            *
            * Select the first application matching the selection AID whatever the SE communication
            * protocol keep the logical channel open after the selection
            */

        /*
            * Generic selection: configures a SeSelector with all the desired attributes to make
            * the selection and read additional information afterwards
            */
        std::vector<char> aid = ByteArrayUtils::fromHex(seAid);
        std::shared_ptr<SeSelector::AidSelector> aidSelector = std::make_shared<SeSelector::AidSelector>(aid, nullptr);
        std::shared_ptr<SeSelector> selector = std::make_shared<SeSelector>(aidSelector, nullptr, StringHelper::formatSimple("AID: %s", seAid));
        std::shared_ptr<SeSelectionRequest> seSelectionRequest = std::make_shared<SeSelectionRequest>(selector, ChannelState::KEEP_OPEN, std::shared_ptr<SeProtocol>(&ContactlessProtocols::PROTOCOL_ISO14443_4));

        /*
            * Add the selection case to the current selection (we could have added other cases
            * here)
            */
        std::shared_ptr<MatchingSe> matchingSe = seSelection->prepareSelection(seSelectionRequest);

        /*
            * Actual SE communication: operate through a single request the SE selection
            */
        if (seSelection->processExplicitSelection()) {
            std::cout << "The selection of the SE has succeeded." << std::endl;
            std::cout << "Application FCI = " << matchingSe->getSelectionSeResponse()->getSelectionStatus()->getFci() << std::endl;

            std::cout << "==================================================================================" << std::endl;
            std::cout << "= End of the generic SE processing.                                              =" << std::endl;
            std::cout << "==================================================================================" << std::endl;
        }
        else {
            std::cout << "The selection of the SE has failed." << std::endl;
        }
    }
    else {
        std::cout << "No SE were detected." << std::endl;
    }
   
    return 0;
}
