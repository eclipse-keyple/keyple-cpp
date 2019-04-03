#include "AbstractObservableReader.h"
#include "ReaderEvent.h"
#include "SeRequestSet.h"
#include "SeResponseSet.h"
#include "SeRequest.h"
#include "SeResponse.h"
#include "KeypleReaderException.h"
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
#include "SeReader.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "System.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using ObservableReader = org::eclipse::keyple::seproxy::event::ObservableReader;
                    using ReaderEvent = org::eclipse::keyple::seproxy::event::ReaderEvent;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                    using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;

                    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractObservableReader));

                    AbstractObservableReader::AbstractObservableReader(const std::string &pluginName, const std::string &readerName)
                    : AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>(readerName), pluginName(pluginName) , notificationMode(NotificationMode::ALWAYS) {
                        this->before = System::nanoTime(); /*
                                                                                  * provides an initial value for measuring the
                                                                                  * inter-exchange time. The first measurement gives the
                                                                                  * time elapsed since the plugin was loaded
                                                                                  */
                    }

                    void AbstractObservableReader::startObservation()
                    {
                        logger->debug("\n");
                    };

                    void AbstractObservableReader::stopObservation() {
                    };

                    std::string AbstractObservableReader::getPluginName() {
                        return pluginName;
                        if (AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::countObservers() == 0) {
                            logger->debug("stop the reader monitoring\n");
                            stopObservation();
                        }
                    }

                    int AbstractObservableReader::compareTo(std::shared_ptr<SeReader> seReader) {
                        return AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName().compare(seReader->getName());
                    }

                    std::shared_ptr<SeResponseSet> AbstractObservableReader::transmitSet(std::shared_ptr<SeRequestSet> requestSet) throw(KeypleReaderException) {
                        if (requestSet == nullptr) {
                            throw std::invalid_argument("seRequestSet must not be null\n");
                        }

                        std::shared_ptr<SeResponseSet> responseSet;

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[%s] transmit => SEREQUESTSET = %s, elapsed %d ms\n",
                                          AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), requestSet->toString(), elapsedMs);
                        }

                        try {
                            responseSet = processSeRequestSet(requestSet);
                        }
                        catch (const KeypleChannelStateException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("transmit => SEREQUESTSET channel failure. elapsed %d\n", elapsedMs);
                            /* Throw an exception with the responses collected so far. */
                            throw ex;
                        }
                        catch (const KeypleIOReaderException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("transmit => SEREQUESTSET IO failure. elapsed %d\n", elapsedMs);
                            /* Throw an exception with the responses collected so far. */
                            throw ex;
                        }

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[%s] transmit => SERESPONSESET = %s, elapsed %d ms\n",
                                          AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), responseSet->toString(), elapsedMs);
                        }

                        return responseSet;
                    }

                    std::shared_ptr<SeResponse> AbstractObservableReader::transmit(std::shared_ptr<SeRequest> seRequest) throw(KeypleReaderException) {
                        if (seRequest == nullptr) {
                            throw std::invalid_argument("seRequest must not be null\n");
                        }

                        std::shared_ptr<SeResponse> seResponse = nullptr;

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[%s] transmit => SEREQUEST = %s, elapsed %d ms\n",
                                          AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), seRequest->toString(), elapsedMs);
                        }

                        try {
                            seResponse = processSeRequest(seRequest);
                        }
                        catch (const KeypleChannelStateException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[%s] transmit => SEREQUEST channel failure. elapsed %d\n",
                                          AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), elapsedMs);
                            /* Throw an exception with the responses collected so far (ex.getSeResponse()). */
                            throw ex;
                        }
                        catch (const KeypleIOReaderException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[%s] transmit => SEREQUEST IO failure. elapsed %d\n",
                                          AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), elapsedMs);
                            /* Throw an exception with the responses collected so far (ex.getSeResponse()). */
                            throw ex;
                        }

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[%s] transmit => SERESPONSE = %s, elapsed %d ms\n",
                                          AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::getName(), seResponse->toString(), elapsedMs);
                        }

                        return seResponse;
                    }

                    void AbstractObservableReader::addObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) {
                        // if an observer is added to an empty list, start the observation
                        if (AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::countObservers() == 0) {
                            logger->debug("Start the reader monitoring.");
                            startObservation();
                        }
                        AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::addObserver(
                            std::dynamic_pointer_cast<org::eclipse::keyple::util::Observer<std::shared_ptr<ReaderEvent>>>(observer));
                    }

                    void AbstractObservableReader::removeObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) {
                        AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::removeObserver(
                            std::dynamic_pointer_cast<org::eclipse::keyple::util::Observer<std::shared_ptr<ReaderEvent>>>(observer));
                        if (AbstractLoggedObservable<std::shared_ptr<ReaderEvent>>::countObservers() == 0) {
                            logger->debug("Stop the reader monitoring.");
                            stopObservation();
                        }
                    }
                }
            }
        }
    }
}
