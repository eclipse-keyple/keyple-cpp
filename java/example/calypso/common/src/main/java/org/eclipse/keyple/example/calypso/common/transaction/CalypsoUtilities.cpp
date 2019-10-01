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
#include "SeSelector.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace org::eclipse::keyple::example::calypso::common::postructure;
using namespace org::eclipse::keyple::example::generic::pc;
using namespace org::eclipse::keyple::core::selection;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::exception;
using namespace org::eclipse::keyple::core::seproxy::protocol;

std::shared_ptr<Properties> CalypsoUtilities::properties;

CalypsoUtilities::StaticConstructor::StaticConstructor()
{
    properties = std::make_shared<Properties>();

    std::string propertiesFileName = "config.properties";

    std::ifstream inputStream;
    inputStream.open(propertiesFileName, std::ifstream::in);
    if (!inputStream) {
        std::cout << "error opening file" << std::endl;
    }

    try {
        properties->load(inputStream);
    }
    catch (const FileNotFoundException &e) {
        e.printStackTrace();
    }
    catch (const IOException &e) {
        e.printStackTrace();
    }
}

CalypsoUtilities::StaticConstructor CalypsoUtilities::staticConstructor;

std::shared_ptr<SeReader> CalypsoUtilities::getDefaultPoReader()
{
    std::shared_ptr<SeReader> poReader = ReaderUtilities::getReaderByName(properties->getProperty("po.reader.regex"));

    ReaderUtilities::setContactlessSettings(poReader);

    return poReader;
}

std::shared_ptr<SamResource> CalypsoUtilities::getDefaultSamResource()
{
    std::shared_ptr<SeReader> samReader = ReaderUtilities::getReaderByName(properties->getProperty("sam.reader.regex"));

    ReaderUtilities::setContactsSettings(samReader);

    /*
     * Open logical channel for the SAM inserted in the reader
     *
     * (We expect the right is inserted)
     */
    return checkSamAndOpenChannel(samReader);
}

std::shared_ptr<SecuritySettings> CalypsoUtilities::getSecuritySettings()
{
    /* define the security parameters to provide when creating PoTransaction */
    return std::make_shared<SecuritySettings>();
}

std::shared_ptr<SamResource> CalypsoUtilities::checkSamAndOpenChannel(std::shared_ptr<SeReader> samReader)
{
    /*
     * check the availability of the SAM doing a ATR based selection, open its physical and
     * logical channels and keep it open
     */
    std::shared_ptr<SeSelection> samSelection = std::make_shared<SeSelection>();
    std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::C1, ".*", "Selection SAM C1");

    /* Prepare selector, ignore MatchingSe here */

    samSelection->prepareSelection(std::make_shared<SamSelectionRequest>(samSelector, ChannelState::KEEP_OPEN));
    std::shared_ptr<CalypsoSam> calypsoSam = nullptr;

    try {
        calypsoSam = std::dynamic_pointer_cast<CalypsoSam>(
                samSelection->processExplicitSelection(samReader)->getActiveSelection()->getMatchingSe());
        if (!calypsoSam->isSelected()) {
            throw IllegalStateException("Unable to open a logical channel for SAM!");
        } else {
        }
    } catch (const KeypleReaderException &e) {
        throw IllegalStateException(StringHelper::formatSimple("Reader exception: %s", e.what()));
    }

    return std::make_shared<SamResource>(samReader, calypsoSam);
}

}
}
}
}
}
}
}
