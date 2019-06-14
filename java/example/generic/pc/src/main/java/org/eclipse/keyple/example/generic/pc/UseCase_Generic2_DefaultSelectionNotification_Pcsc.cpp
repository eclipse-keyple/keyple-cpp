#include "ByteArrayUtils.h"
#include "ContactlessProtocols_Import.h"
#include "KeypleBaseException.h"
#include "PcscPlugin.h"
#include "ReaderEvent_Import.h"
#include "ReaderUtilities.h"
#include "SeSelector.h"
#include "SeSelection.h"
#include "SeSelectionRequest.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

/* Smartcard I/O */
#include "ChannelState.h"

/* Examples */
#include "ReaderUtilities.h"

using ByteArrayUtils       = org::eclipse::keyple::util::ByteArrayUtils;
using ChannelState         = org::eclipse::keyple::seproxy::ChannelState;
using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
using KeypleBaseException  = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
using Logger               = org::eclipse::keyple::common::Logger;
using LoggerFactory        = org::eclipse::keyple::common::LoggerFactory;
using ObservableReader     = org::eclipse::keyple::seproxy::event::ObservableReader;
using PcscPlugin           = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
using ReaderEvent          = org::eclipse::keyple::seproxy::event::ReaderEvent;
using ReaderObserver       = org::eclipse::keyple::seproxy::event::ObservableReader::ReaderObserver;
using ReaderUtilities      = org::eclipse::keyple::example::generic::pc::ReaderUtilities;
using SeProxyService       = org::eclipse::keyple::seproxy::SeProxyService;
using SeReader             = org::eclipse::keyple::seproxy::SeReader;
using SeSelector           = org::eclipse::keyple::seproxy::SeSelector;
using SeSelection          = org::eclipse::keyple::transaction::SeSelection;
using SeSelectionRequest   = org::eclipse::keyple::transaction::SeSelectionRequest;
using SeProtocol           = org::eclipse::keyple::seproxy::protocol::SeProtocol;

class UseCase_Generic2_DefaultSelectionNotification_Pcsc
: public std::enable_shared_from_this<UseCase_Generic2_DefaultSelectionNotification_Pcsc>, public ObservableReader::ReaderObserver  {
private:
    const std::shared_ptr<void> waitForEnd = nullptr;

    //std::string seAid = "A0000004040125090101";
    std::string seAid = "304554502E494341";

    std::shared_ptr<SeSelection> seSelection;
    std::shared_ptr<SeReader> seReader;

public:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(UseCase_Generic2_DefaultSelectionNotification_Pcsc));

    UseCase_Generic2_DefaultSelectionNotification_Pcsc()
    {
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
        seReader = ReaderUtilities::getDefaultContactLessSeReader(shared_proxy);

        /* Check if the reader exists */
        if (seReader == nullptr) {
            throw std::make_shared<IllegalStateException>("Bad SE reader setup");
        }

        logger->info("=============== UseCase Generic #2: AID based default selection ===================\n");
        logger->info("= SE Reader  NAME = %s\n", seReader->getName());

        /*
         * Prepare a SE selection
         */
        seSelection = std::make_shared<SeSelection>(seReader);

        /*
         * Setting of an AID based selection
         *
         * Select the first application matching the selection AID whatever the SE communication
         * protocol keep the logical channel open after the selection
         */

        /*
         * Generic selection: configures a SeSelector with all the desired attributes to make the
         * selection
         */
        std::vector<char> aid = ByteArrayUtils::fromHex(seAid);
        std::shared_ptr<SeSelector::AidSelector> shared_aid = std::make_shared<SeSelector::AidSelector>(aid, nullptr);
        std::shared_ptr<SeSelector> shared_selector = std::make_shared<SeSelector>(shared_aid, nullptr, "AID: " + seAid);
        std::shared_ptr<SeSelectionRequest> seSelector = std::make_shared<SeSelectionRequest>(shared_selector, ChannelState::KEEP_OPEN, std::dynamic_pointer_cast<SeProtocol>(std::shared_ptr<ContactlessProtocols>(&ContactlessProtocols::PROTOCOL_ISO14443_4)));

        /*
         * Add the selection case to the current selection (we could have added other cases here)
         */
        seSelection->prepareSelection(seSelector);

        /*
         * Provide the SeReader with the selection operation to be processed when a SE is inserted.
         */
        (std::dynamic_pointer_cast<ObservableReader>(seReader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);
    }

    void doSomething() 
    {
        /* Set the current class as Observer of the first reader */
        //JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
        (std::dynamic_pointer_cast<ObservableReader>(seReader))->addObserver(shared_from_this());

        logger->info("==================================================================================\n");
        logger->info("= Wait for a SE. The default AID based selection to be processed as soon as the  =\n");
        logger->info("= SE is detected.                                                                =\n");
        logger->info("==================================================================================\n");

        /* Wait for ever (exit with CTRL-C) */
        while(1);
    }

    void update(std::shared_ptr<ReaderEvent> event)
    {
        if (event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
            if (seSelection->processDefaultSelection(event->getDefaultSelectionResponse())) {
                //std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe(); Alex: unused?

                logger->info("Observer notification: the selection of the SE has succeeded\n");

                logger->info("==================================================================================\n");
                logger->info("= End of the SE processing.                                                      =\n");
                logger->info("==================================================================================\n");
            } else {
                logger->error("The selection of the SE has failed. Should not have occurred due to the MATCHED_ONLY selection mode\n");
            }
        } else if (event->getEventType() == ReaderEvent::EventType::SE_INSERTED) {
            logger->error("SE_INSERTED event: should not have occurred due to the MATCHED_ONLY selection mode\n");
        } else if (event->getEventType() == ReaderEvent::EventType::SE_REMOVAL) {
            logger->info("The SE has been removed.");
        }
    }

};

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Create the observable object to handle the SE processing */
    std::shared_ptr<UseCase_Generic2_DefaultSelectionNotification_Pcsc> m = std::make_shared<UseCase_Generic2_DefaultSelectionNotification_Pcsc>();
    m->doSomething();
}
