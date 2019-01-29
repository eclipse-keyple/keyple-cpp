/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "PluginEvent.h"
#include "AbstractObservableReader.h"
#include "KeypleReaderException.h"
#include "InterruptedException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace seproxy {
namespace plugin {

using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
using PluginEvent = org::eclipse::keyple::seproxy::event::PluginEvent;
using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractThreadedObservablePlugin));
std::set<std::string> _set;
std::shared_ptr<std::set<std::string>> nativeReadersNames = std::make_shared<std::set<std::string>>(_set);

AbstractThreadedObservablePlugin::AbstractThreadedObservablePlugin(const std::string &name) : AbstractObservablePlugin(name) {
}

void AbstractThreadedObservablePlugin::startObservation()
{
    thread = std::shared_ptr<EventThread>(new EventThread(shared_from_this(), this->getName()));
    thread->start();
}

void AbstractThreadedObservablePlugin::stopObservation()
{
    if (thread != nullptr) {
        thread->end();
    }
}

AbstractThreadedObservablePlugin::EventThread::EventThread(std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance, const std::string &pluginName) : pluginName(pluginName), outerInstance(outerInstance) {
}

void AbstractThreadedObservablePlugin::EventThread::end() {
    running = false;
    this->interrupt();
}

void AbstractThreadedObservablePlugin::EventThread::run() {
    try {
        while (running) {
            /* retrieves the current readers names list */
            std::shared_ptr<std::set<std::string>> actualNativeReadersNames = outerInstance->getNativeReadersNames();
            /*
             * checks if it has changed this algorithm favors cases where nothing change
             */
            if (outerInstance->nativeReadersNames != actualNativeReadersNames) {
                /*
                 * parse the current readers list, notify for disappeared readers, update
                 * readers list
                 */
                for (auto reader : *outerInstance->readers) {
                    if (actualNativeReadersNames->find(reader->getName()) == actualNativeReadersNames->end()) {
                        outerInstance->AbstractLoggedObservable::notifyObservers(std::make_shared<PluginEvent>(this->pluginName, reader->getName(), PluginEvent::EventType::READER_DISCONNECTED));
                        outerInstance->readers->erase(reader);
                        outerInstance->logger->trace("[{}][{}] Plugin thread => Remove unplugged reader from readers list.", this->pluginName, reader->getName());
                        /* remove reader name from the current list */
                        outerInstance->nativeReadersNames->erase(reader->getName());
                        reader.reset();
                    }
                }
                /*
                 * parse the new readers list, notify for readers appearance, update readers
                 * list
                 */
                for (auto readerName : *actualNativeReadersNames) {
                    if (outerInstance->nativeReadersNames->find(readerName) == outerInstance->nativeReadersNames->end()) {
                        std::shared_ptr<SeReader> reader = outerInstance->getNativeReader(readerName);
                        outerInstance->readers->insert(reader);
                        outerInstance->AbstractLoggedObservable::notifyObservers(std::make_shared<PluginEvent>(this->pluginName, reader->getName(), PluginEvent::EventType::READER_CONNECTED));
                        outerInstance->logger->trace("[{}][{}] Plugin thread => Add plugged reader to readers list.", this->pluginName, reader->getName());
                        /* add reader name to the current list */
                        outerInstance->nativeReadersNames->insert(readerName);
                    }
                }
            }
            /* sleep for a while. */
            Thread::sleep(outerInstance->threadWaitTimeout);
        }
    }
    catch (InterruptedException &e) {
        e.printStackTrace();
        outerInstance->logger->warn("[{}] An exception occurred while monitoring plugin: {}, cause {}", this->pluginName, e.getMessage(), e.getCause());
    }
    catch (KeypleReaderException &e) {
        e.printStackTrace();
        outerInstance->logger->warn("[{}] An exception occurred while monitoring plugin: {}, cause {}", this->pluginName, e.what(), e.getCause());
    }
}

//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
void AbstractThreadedObservablePlugin::finalize() throw(std::runtime_error) {
    thread->end();
    thread.reset();
    logger->trace("[{}] Observable Plugin thread ended.", this->getName());
    /*
     * Alex...
     */
    //AbstractObservablePlugin::finalize();
}

}
}
}
}
}
