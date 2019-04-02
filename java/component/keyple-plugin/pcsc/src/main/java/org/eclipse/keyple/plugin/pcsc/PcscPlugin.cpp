/* Core */
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "AbstractObservableReader.h"

/* PC/SC plugin */
#include "PcscReader.h"
#include "PcscPlugin.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace plugin {
namespace pcsc {

using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;

std::shared_ptr<TerminalFactory> PcscPlugin::factory;

PcscPlugin::PcscPlugin() : org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin("PcscPlugin")
{
    logger->debug("constructor\n");
}

PcscPlugin::~PcscPlugin()
{
    logger->debug("destructor\n");
}

PcscPlugin PcscPlugin::getInstance()
{
    static PcscPlugin uniqueInstance;

    return uniqueInstance;
}

std::unordered_map<std::string, std::string> PcscPlugin::getParameters() {
     return std::unordered_map<std::string, std::string>();
}

                    void PcscPlugin::setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument, KeypleBaseException) {

}

std::shared_ptr<PcscPlugin> PcscPlugin::setLogging(bool logging)
{
    this->logging = logging;
                        return shared_from_this();
}

std::shared_ptr<std::set<std::string>> PcscPlugin::fetchNativeReadersNames() throw(KeypleReaderException)
{
    nativeReadersNames.clear();
    std::shared_ptr<CardTerminals> terminals = getCardTerminals();

    logger->debug("filling native readers name list with card terminal items\n");
    try {
        for (auto &term : terminals->list()) {
            logger->debug("reader: %s\n", term.getName());
            nativeReadersNames.insert(term.getName());
        }
    } catch (CardException &e) {
        std::string cause = e.getCause().what();
        if (!cause.compare("SCARD_E_NO_READERS_AVAILABLE")) {
            logger->trace("no reader available\n");
        } else {
            logger->trace("terminal list is not accessible, name: %s, exception: %s\n", this->getName(), e.getMessage());
            throw KeypleReaderException("Could not access terminals list"); //, e);
        }
    }

    return std::make_shared<std::set<std::string>>(nativeReadersNames);
}

std::shared_ptr<std::set<std::shared_ptr<SeReader>>> PcscPlugin::initNativeReaders() throw(KeypleReaderException)
{
    logger->debug("creating new list\n");
    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> nativeReaders = std::shared_ptr<std::set<std::shared_ptr<SeReader>>>(new std::set<std::shared_ptr<SeReader>>());

    // parse the current readers list to create the ProxyReader(s) associated with new reader(s)
    logger->debug("getting card terminals\n");
    std::shared_ptr<CardTerminals> terminals = getCardTerminals();
    try {
        for (auto &term : terminals->list()) {
            logger->debug("inserting card terminals into list\n");
            nativeReaders->insert(std::make_shared<PcscReader>(this->getName(), std::make_shared<CardTerminal>(term)));
        }
    }
    catch (CardException &e) {
        logger->debug("CardException\n");
        std::string cause = e.getCause().what();
        if (!cause.compare("SCARD_E_NO_READERS_AVAILABLE")) {
            logger->trace("No reader available.");
        }
        else {
            logger->trace("[%s] terminal list is not accessible, exception: %s", this->getName(), e.getMessage());
            throw KeypleReaderException("Could not access terminals list"); //, e);

        }
    }

    return nativeReaders;
}

std::shared_ptr<AbstractObservableReader> PcscPlugin::fetchNativeReader(const std::string &name) throw(KeypleReaderException) {
    // return the current reader if it is already listed
    for (auto reader : *readers) {
        if (reader->getName() == name) {
            return std::dynamic_pointer_cast<AbstractObservableReader>(reader);
        }
    }
    /*
     * parse the current PC/SC readers list to create the ProxyReader(s) associated with new
     * reader(s)
     */
    std::shared_ptr<AbstractObservableReader> reader = nullptr;
    std::shared_ptr<CardTerminals> terminals = getCardTerminals();
    std::vector<std::string> terminalList;
    try {
        for (auto &term : terminals->list()) {
            if (!term.getName().compare(name)) {
                reader = std::make_shared<PcscReader>(this->getName(), std::make_shared<CardTerminal>(term));
            }
        }
    }
    catch (CardException &e) {
        logger->trace("[{}] Terminal list is not accessible. Exception: {}", this->getName(), e.getMessage());
        throw KeypleReaderException("Could not access terminals list"); //, e);
    }
    if (reader == nullptr) {
        throw KeypleReaderException("Reader " + name + " not found!");
    }
    return reader;
}

std::shared_ptr<CardTerminals> PcscPlugin::getCardTerminals() {
    // try {
    // Class pcscterminal = null;
    // pcscterminal = Class.forName("sun.security.smartcardio.PCSCTerminals");
    // Field contextId = pcscterminal.getDeclaredField("contextId");
    // contextId.setAccessible(true);
    //
    // if (contextId.getLong(pcscterminal) != 0L) {
    // Class pcsc = Class.forName("sun.security.smartcardio.PCSC");
    // Method SCardEstablishContext =
    // pcsc.getDeclaredMethod("SCardEstablishContext", new Class[] {Integer.TYPE});
    // SCardEstablishContext.setAccessible(true);
    //
    // Field SCARD_SCOPE_USER = pcsc.getDeclaredField("SCARD_SCOPE_USER");
    // SCARD_SCOPE_USER.setAccessible(true);
    //
    // long newId = ((Long) SCardEstablishContext.invoke(pcsc,
    // new Object[] {Integer.valueOf(SCARD_SCOPE_USER.getInt(pcsc))})).longValue();
    // contextId.setLong(pcscterminal, newId);
    // }
    // } catch (Exception e) {
    // e.printStackTrace();
    // }

    if (factory == nullptr) {
        factory = TerminalFactory::getDefault();
    }

    std::shared_ptr<CardTerminals> terminals = factory->terminals();

    return terminals;
}

}
}
}
}
}
