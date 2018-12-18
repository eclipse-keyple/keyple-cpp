#include "AbstractThreadedLocalReader.h"
#include "../exception/NoStackTraceThrowable.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> AbstractThreadedLocalReader::logger = org::slf4j::LoggerFactory::getLogger(AbstractThreadedLocalReader::typeid);
const std::shared_ptr<java::util::concurrent::atomic::AtomicInteger> AbstractThreadedLocalReader::threadCount = std::make_shared<java::util::concurrent::atomic::AtomicInteger>();

                    AbstractThreadedLocalReader::AbstractThreadedLocalReader(const std::string &pluginName, const std::string &readerName) : AbstractSelectionLocalReader(pluginName, readerName) {
                    }

                    void AbstractThreadedLocalReader::startObservation() {
                        thread = std::make_shared<EventThread>(shared_from_this(), this->getPluginName(), this->getName());
                        thread->start();
                    }

                    void AbstractThreadedLocalReader::stopObservation() {
                        thread->end();
                    }

                    void AbstractThreadedLocalReader::setThreadWaitTimeout(long long timeout) {
                        this->threadWaitTimeout = timeout;
                    }

                    AbstractThreadedLocalReader::EventThread::EventThread(std::shared_ptr<AbstractThreadedLocalReader> outerInstance, const std::string &pluginName, const std::string &readerName) : Thread("observable-reader-events-" + threadCount->addAndGet(1)), pluginName(pluginName), readerName(readerName), outerInstance(outerInstance) {
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
                            if (isSePresent()) {
                                outerInstance->cardInserted();
                            }

                            while (running) {
                                // If we have a card,
                                if (isSePresent()) {
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
                            logger->trace("[{}] Exception occurred in monitoring thread: {}", readerName, e->what());
                        }
                    }

//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
                    void AbstractThreadedLocalReader::finalize() throw(std::runtime_error) {
                        thread->end();
                        thread.reset();
                        logger->trace("[{}] Observable Reader thread ended.", this->getName());
                        AbstractSelectionLocalReader::finalize();
                    }
                }
            }
        }
    }
}
