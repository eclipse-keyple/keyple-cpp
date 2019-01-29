#include "PcscPlugin.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservableReader.h"
#include "PcscReader.h"

/* Common */
#include "LoggerFactory.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;

                    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(PcscPlugin));
                    const std::shared_ptr<PcscPlugin> PcscPlugin::uniqueInstance = std::shared_ptr<PcscPlugin>(new PcscPlugin());
                    std::shared_ptr<TerminalFactory> PcscPlugin::factory;

                    PcscPlugin::PcscPlugin() : org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin("PcscPlugin") {
                    }

                    std::shared_ptr<PcscPlugin> PcscPlugin::getInstance() {
                        return uniqueInstance;
                    }

                    std::unordered_map<std::string, std::string> PcscPlugin::getParameters() {
                        return this->getParameters();
                    }

                    void PcscPlugin::setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument, KeypleBaseException) {

                    }

                    std::shared_ptr<PcscPlugin> PcscPlugin::setLogging(bool logging) {
                        this->logging = logging;
                        return std::dynamic_pointer_cast<PcscPlugin>(shared_from_this());
                    }

                    std::shared_ptr<std::set<std::string>> PcscPlugin::getNativeReadersNames() throw(KeypleReaderException) {
                        std::shared_ptr<std::set<std::string>> nativeReadersNames = std::shared_ptr<std::set<std::string>>(new std::set<std::string>());
                        std::shared_ptr<CardTerminals> terminals = getCardTerminals();
                        try {
                            for (auto &term : terminals->list()) {
                                nativeReadersNames->insert(term.getName());
                            }
                        }
                        catch (CardException &e) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            if (e.getCause().find("SCARD_E_NO_READERS_AVAILABLE") != std::string::npos) {
                                logger->trace("No reader available.");
                            }
                            else {
                                logger->trace("[{}] getNativeReadersNames => Terminal list is not accessible. Exception: {}", this->getName(), e.getMessage());
                                throw std::shared_ptr<KeypleReaderException>(new KeypleReaderException("Could not access terminals list")); // Alex: 'e' should be in exception constructor but...
                            }
                        }
                        return nativeReadersNames;
                    }

                    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> PcscPlugin::getNativeReaders() throw(KeypleReaderException) {
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>> nativeReaders = std::shared_ptr<std::set<std::shared_ptr<SeReader>>>(new std::set<std::shared_ptr<SeReader>>());

                        // parse the current readers list to create the ProxyReader(s) associated with new reader(s)
                        std::shared_ptr<CardTerminals> terminals = getCardTerminals();
                        try {
                            for (auto &term : terminals->list()) {
                                nativeReaders->insert(std::shared_ptr<PcscReader>(new PcscReader(this->getName(), std::shared_ptr<CardTerminal>(&term))));
                            }
                        }
                        catch (CardException &e) {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            if (e.getCause().find("SCARD_E_NO_READERS_AVAILABLE") != std::string::npos) {
                                logger->trace("No reader available.");
                            }
                            else {
                                logger->trace("[{}] Terminal list is not accessible. Exception: {}", this->getName(), e.getMessage());
                                throw std::shared_ptr<KeypleReaderException>(new KeypleReaderException("Could not access terminals list")); //, e));

                            }
                        }
                        return nativeReaders;
                    }

                    std::shared_ptr<SeReader> PcscPlugin::getNativeReader(const std::string &name) throw(KeypleReaderException) {
                        // return the current reader if it is already listed
                        for (auto reader : *readers) {
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
                            for (auto &term : terminals->list()) {
                                if (!term.getName().compare(name)) {
                                    reader = std::shared_ptr<PcscReader>(new PcscReader(this->getName(), std::make_shared<CardTerminal>(term)));
                                }
                            }
                        }
                        catch (CardException &e) {
                            logger->trace("[{}] Terminal list is not accessible. Exception: {}", this->getName(), e.getMessage());
                            throw std::shared_ptr<KeypleReaderException>(new KeypleReaderException("Could not access terminals list")); //, e));
                        }
                        if (reader == nullptr) {
                            throw std::shared_ptr<KeypleReaderException>(new KeypleReaderException("Reader " + name + " not found!"));
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
