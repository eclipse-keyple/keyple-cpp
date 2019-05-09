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

                    void AbstractThreadedLocalReader::startObservation()
                    {
                        logger->debug("starting observation\n");
                        thread = std::make_shared<EventThread>(shared_from_this(), this->getPluginName(), AbstractLoggedObservable<ReaderEvent>::getName());
                        thread->start();
                    }

                    void AbstractThreadedLocalReader::stopObservation()
                    {
                        logger->debug("stopping observation\n");
                        thread->end();
                    }

                    void AbstractThreadedLocalReader::setThreadWaitTimeout(long long timeout)
                    {
                        logger->debug("setting thread timeout to %d ms", timeout);
                        this->threadWaitTimeout = timeout;
                    }

                    AbstractThreadedLocalReader::EventThread::EventThread(std::shared_ptr<AbstractThreadedLocalReader> outerInstance, const std::string &pluginName, const std::string &readerName)
                    : Thread("observable-reader-events-" + std::to_string(++(outerInstance->threadCount))), outerInstance(outerInstance), pluginName(pluginName), readerName(readerName)
                    {
                        outerInstance->logger->debug("constructor\n");

                        setDaemon(true);
                    }

                    void AbstractThreadedLocalReader::EventThread::end()
                    {
                        outerInstance->logger->debug("stopping thread\n");
                        running = false;
                        this->interrupt(); // exit io wait if needed
                    }

                    void *AbstractThreadedLocalReader::EventThread::run()
                    {
                        outerInstance->logger->debug("starting thread (run)\n");

                        try {
                            // First thing we'll do is to notify that a card was inserted if one is already
                            // present.
                            outerInstance->logger->debug("checking if a SE is present...\n");
                            if (outerInstance->isSePresent()) {
                                outerInstance->logger->debug("notify card inserted\n");
                                outerInstance->cardInserted();
                            }

                            while (running) {
                                // If we have a card,
                                outerInstance->logger->debug("checking if a SE is present...\n");
                                if (outerInstance->isSePresent()) {
                                    // we will wait for it to disappear
                                    outerInstance->logger->debug("waiting for card to be removed...\n");
                                    if (outerInstance->waitForCardAbsent(outerInstance->threadWaitTimeout)) {
                                        // and notify about it.
                                        outerInstance->logger->debug("notify card removed\n");
                                        outerInstance->cardRemoved();
                                    }
                                    // false means timeout, and we go back to the beginning of the loop
                                }
                                // If we don't,
                                else {
                                    // we will wait for it to appear
                                    outerInstance->logger->debug("waiting for card to be inserted...\n");
                                    if (outerInstance->waitForCardPresent(outerInstance->threadWaitTimeout)) {
                                        outerInstance->logger->debug("notify card inserted\n");
                                        outerInstance->cardInserted();
                                    }
                                    // false means timeout, and we go back to the beginning of the loop
                                }
                            }
                        }
                        catch (const NoStackTraceThrowable &e) {
                            outerInstance->logger->trace("[%s] exception occurred in monitoring thread: %s\n", readerName, e.what());
                        }

                        return NULL;
                    }

//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
                    void AbstractThreadedLocalReader::finalize()
                    {
                        thread->end();
                        thread.reset();
                        logger->trace("[%s] Observable Reader thread ended\n", AbstractLoggedObservable<ReaderEvent>::getName());
                        //AbstractSelectionLocalReader::finalize();
                    }
                }
            }
        }
    }
}
