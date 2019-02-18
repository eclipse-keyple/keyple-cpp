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

AbstractThreadedObservablePlugin::AbstractThreadedObservablePlugin(const std::string &name)
: AbstractObservablePlugin(name)
{
    logger->debug("[AbstractThreadedObservablePlugin::AbstractThreadedObservablePlugin] name: %s\n", name);
}

void AbstractThreadedObservablePlugin::startObservation()
{
    logger->debug("[AbstractThreadedObservablePlugin::startObservation]\n");

    thread = std::shared_ptr<EventThread>(new EventThread(shared_from_this(), this->getName()));
    thread->start();
}

void AbstractThreadedObservablePlugin::stopObservation()
{
    logger->debug("[AbstractThreadedObservablePlugin::stopObservation]\n");

    if (thread != nullptr) {
        thread->end();
    }
}

AbstractThreadedObservablePlugin::EventThread::EventThread(std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance, const std::string &pluginName) : pluginName(pluginName), outerInstance(outerInstance)
{
    outerInstance->logger->debug("[AbstractThreadedObservablePlugin::EventThread] constructor with outerInstance: %p, pluginName: %s\n", outerInstance, pluginName);
}

void AbstractThreadedObservablePlugin::EventThread::end()
{
    outerInstance->logger->debug("[AbstractThreadedObservablePlugin::EventThread::end]\n");

    running = false;
    this->interrupt();
}

void *AbstractThreadedObservablePlugin::EventThread::run()
{
    static const char *f_name = "AbstractThreadedObservablePlugin::EventThread::run";

    outerInstance->logger->debug("[%s]\n", f_name);

    try {
        outerInstance->logger->debug("[%s] running: %d\n", f_name, running);
        while (running) {
            /* retrieves the current readers names list */
            outerInstance->logger->debug("[%s] retrieving current readers list\n", f_name);
            std::shared_ptr<std::set<std::string>> actualNativeReadersNames = outerInstance->getNativeReadersNames();
            outerInstance->logger->debug("[%s] actualNativeReadersNames: %p, size: %d\n",
                                         f_name, actualNativeReadersNames,
                                         actualNativeReadersNames->size());

            /*
             * checks if it has changed this algorithm favors cases where nothing change
             */
            outerInstance->logger->debug("[%s] nativeReadersNames: %p\n",
                                         f_name, outerInstance->nativeReadersNames);
            if (outerInstance->nativeReadersNames != actualNativeReadersNames) {
                /*
                 * parse the current readers list, notify for disappeared readers, update
                 * readers list
                 */
                outerInstance->logger->debug("[%s] updating current list (removal)\n", f_name);
                outerInstance->logger->debug("[%s] readers: %p\n", f_name, outerInstance->readers);
                for (auto reader : *outerInstance->readers) {
                    outerInstance->logger->debug("[%s] reader: %p\n", f_name, reader);
                    if (actualNativeReadersNames->find(reader->getName()) == actualNativeReadersNames->end()) {
                        outerInstance->AbstractLoggedObservable::notifyObservers(std::make_shared<PluginEvent>(this->pluginName, reader->getName(), PluginEvent::EventType::READER_DISCONNECTED));
                        outerInstance->readers->erase(reader);
                        outerInstance->logger->trace("[%s] plugin thread => remove unplugged reader from readers list.", f_name, this->pluginName, reader->getName());
                        /* remove reader name from the current list */
                        outerInstance->nativeReadersNames->erase(reader->getName());
                        reader.reset();
                    }
                }

                /*
                 * parse the new readers list, notify for readers appearance, update readers
                 * list
                 */
                outerInstance->logger->debug("[AbstractThreadedObservablePlugin::EventThread::run] updating current list (addition)\n");
                for (auto readerName : *actualNativeReadersNames) {
                    if (outerInstance->nativeReadersNames->find(readerName) == outerInstance->nativeReadersNames->end()) {
                        outerInstance->logger->debug("[AbstractThreadedObservablePlugin::EventThread::run]\n");
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
            outerInstance->logger->debug("[AbstractThreadedObservablePlugin::EventThread::run] sleeping for a bit (%d ms)\n", outerInstance->threadWaitTimeout);
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

    return NULL;
}

//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
void AbstractThreadedObservablePlugin::finalize() throw(std::runtime_error)
{
    logger->debug("[AbstractThreadedObservablePlugin::finalize]\n");

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
