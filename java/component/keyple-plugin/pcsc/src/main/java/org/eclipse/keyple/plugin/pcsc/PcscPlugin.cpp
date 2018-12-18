#include "PcscPlugin.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"
#include "PcscReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> PcscPlugin::logger = org::slf4j::LoggerFactory::getLogger(PcscPlugin::typeid);
const std::shared_ptr<PcscPlugin> PcscPlugin::uniqueInstance = std::make_shared<PcscPlugin>();
std::shared_ptr<javax::smartcardio::TerminalFactory> PcscPlugin::factory;

                    PcscPlugin::PcscPlugin() : org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin("PcscPlugin") {
                    }

                    std::shared_ptr<PcscPlugin> PcscPlugin::getInstance() {
                        return uniqueInstance;
                    }

                    std::unordered_map<std::string, std::string> PcscPlugin::getParameters() {
                        return nullptr;
                    }

                    void PcscPlugin::setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument, KeypleBaseException) {

                    }

                    std::shared_ptr<PcscPlugin> PcscPlugin::setLogging(bool logging) {
                        this->logging = logging;
                        return shared_from_this();
                    }

                    std::shared_ptr<SortedSet<std::string>> PcscPlugin::getNativeReadersNames() throw(KeypleReaderException) {
                        std::shared_ptr<SortedSet<std::string>> nativeReadersNames = std::make_shared<ConcurrentSkipListSet<std::string>>();
                        std::shared_ptr<CardTerminals> terminals = getCardTerminals();
                        try {
                            for (std::shared_ptr<CardTerminal> term : terminals->list()) {
                                nativeReadersNames->add(term->getName());
                            }
                        }
                        catch (const CardException &e) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            if (e->getCause()->toString()->contains("SCARD_E_NO_READERS_AVAILABLE")) {
                                logger->trace("No reader available.");
                            }
                            else {
                                logger->trace("[{}] getNativeReadersNames => Terminal list is not accessible. Exception: {}", this->getName(), e->getMessage());
                                throw std::make_shared<KeypleReaderException>("Could not access terminals list", e);
                            }
                        }
                        return nativeReadersNames;
                    }

                    std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> PcscPlugin::getNativeReaders() throw(KeypleReaderException) {
                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> nativeReaders = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<AbstractObservableReader>>>();

                        // parse the current readers list to create the ProxyReader(s) associated with new reader(s)
                        std::shared_ptr<CardTerminals> terminals = getCardTerminals();
                        try {
                            for (std::shared_ptr<CardTerminal> term : terminals->list()) {
                                nativeReaders->add(std::make_shared<PcscReader>(this->getName(), term));
                            }
                        }
                        catch (const CardException &e) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            if (e->getCause()->toString()->contains("SCARD_E_NO_READERS_AVAILABLE")) {
                                logger->trace("No reader available.");
                            }
                            else {
                                logger->trace("[{}] Terminal list is not accessible. Exception: {}", this->getName(), e->getMessage());
                                throw std::make_shared<KeypleReaderException>("Could not access terminals list", e);

                            }
                        }
                        return nativeReaders;
                    }

                    std::shared_ptr<AbstractObservableReader> PcscPlugin::getNativeReader(const std::string &name) throw(KeypleReaderException) {
                        // return the current reader if it is already listed
                        for (auto reader : readers) {
                            if (reader->getName() == name) {
                                return reader;
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
                            for (std::shared_ptr<CardTerminal> term : terminals->list()) {
                                if (term->getName().equals(name)) {
                                    reader = std::make_shared<PcscReader>(this->getName(), term);
                                }
                            }
                        }
                        catch (const CardException &e) {
                            logger->trace("[{}] Terminal list is not accessible. Exception: {}", this->getName(), e->getMessage());
                            throw std::make_shared<KeypleReaderException>("Could not access terminals list", e);
                        }
                        if (reader == nullptr) {
                            throw std::make_shared<KeypleReaderException>("Reader " + name + " not found!");
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
