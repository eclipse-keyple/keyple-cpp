#include "AbstractThreadedLocalReader.h"
#include "../exception/NoStackTraceThrowable.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;

                    const std::shared_ptr<Logger> AbstractThreadedLocalReader::logger = nullptr; //LoggerFactory::getLogger(AbstractThreadedLocalReader::typeid);

                    AbstractThreadedLocalReader::AbstractThreadedLocalReader(const std::string &pluginName, const std::string &readerName) : AbstractSelectionLocalReader(pluginName, readerName) {
                    }

                    void AbstractThreadedLocalReader::startObservation() {
                        EventThread *_et = new EventThread(std::shared_ptr<AbstractThreadedLocalReader>(this), this->getPluginName(), this->getName());
                        thread = std::shared_ptr<EventThread>(_et);
                        thread->start();
                    }

                    void AbstractThreadedLocalReader::stopObservation() {
                        thread->end();
                    }

                    void AbstractThreadedLocalReader::setThreadWaitTimeout(long long timeout) {
                        this->threadWaitTimeout = timeout;
                    }

                    AbstractThreadedLocalReader::EventThread::EventThread(std::shared_ptr<AbstractThreadedLocalReader> outerInstance, const std::string &pluginName, const std::string &readerName) : Thread("observable-reader-events-" + std::to_string(++threadCount)), pluginName(pluginName), readerName(readerName), outerInstance(outerInstance) {
                        setDaemon(true);
                    }

                    void AbstractThreadedLocalReader::EventThread::end() {
                        running = false;
                        this->interrupt(); // exit io wait if needed
                    }

                    void AbstractThreadedLocalReader::EventThread::run() {
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
                        catch (NoStackTraceThrowable &e) {
                            logger->trace("[{}] Exception occurred in monitoring thread: {}", readerName, e.what());
                        }
                    }

//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
                    void AbstractThreadedLocalReader::finalize() throw(std::runtime_error) {
                        thread->end();
                        thread.reset();
                        logger->trace("[{}] Observable Reader thread ended.", this->getName());

                        /*
                         * Alex: Not sure which super class is supposed to be
                         * called here. Object class has only the virtual
                         * method.
                         */
                        //finalize();
                    }
                }
            }
        }
    }
}
