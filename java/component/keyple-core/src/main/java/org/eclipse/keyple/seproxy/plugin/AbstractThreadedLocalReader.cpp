/* Core */
#include "AbstractThreadedLocalReader.h"
#include "NoStackTraceThrowable.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;

                    AbstractThreadedLocalReader::AbstractThreadedLocalReader(const std::string &pluginName, const std::string &readerName) : AbstractSelectionLocalReader(pluginName, readerName) {
                    }

                    void AbstractThreadedLocalReader::startObservation() {
                        thread = std::make_shared<EventThread>(shared_from_this(), this->getPluginName(), AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName());
                        thread->start();
                    }

                    void AbstractThreadedLocalReader::stopObservation() {
                        thread->end();
                    }

                    void AbstractThreadedLocalReader::setThreadWaitTimeout(long long timeout) {
                        this->threadWaitTimeout = timeout;
                    }

                    AbstractThreadedLocalReader::EventThread::EventThread(std::shared_ptr<AbstractThreadedLocalReader> outerInstance, const std::string &pluginName, const std::string &readerName)
                    : Thread("observable-reader-events-" + std::to_string(++(outerInstance->threadCount))), outerInstance(outerInstance), pluginName(pluginName), readerName(readerName)
                    {
                        outerInstance->logger->debug("constructor\n");

                        setDaemon(true);
                    }

                    void AbstractThreadedLocalReader::EventThread::end() {
                        running = false;
                        this->interrupt(); // exit io wait if needed
                    }

                    void *AbstractThreadedLocalReader::EventThread::run()
                    {
                        outerInstance->logger->debug("run\n");

                        try {
                            // First thing we'll do is to notify that a card was inserted if one is already
                            // present.
                            if (outerInstance->isSePresent()) {
                                outerInstance->cardInserted();
                            }

                            while (running) {
                                // If we have a card,
                                if (outerInstance->isSePresent()) {
                                    // we will wait for it to disappear
                                    if (outerInstance->waitForCardAbsent(outerInstance->threadWaitTimeout)) {
                                        // and notify about it.
                                        outerInstance->cardRemoved();
                                    }
                                    // false means timeout, and we go back to the beginning of the loop
                                }
                                // If we don't,
                                else {
                                    // we will wait for it to appear
                                    if (outerInstance->waitForCardPresent(outerInstance->threadWaitTimeout)) {
                                        outerInstance->cardInserted();
                                    }
                                    // false means timeout, and we go back to the beginning of the loop
                                }
                            }
                        }
                        catch (const NoStackTraceThrowable &e) {
                            outerInstance->logger->trace("[%s] exception occurred in monitoring thread: %s)", readerName, e.what());
                        }

                        return NULL;
                    }

//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
                    void AbstractThreadedLocalReader::finalize()
                    {
                        thread->end();
                        thread.reset();
                        logger->trace("[{}] Observable Reader thread ended.", AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName());
                        //AbstractSelectionLocalReader::finalize();
                    }
                }
            }
        }
    }
}
