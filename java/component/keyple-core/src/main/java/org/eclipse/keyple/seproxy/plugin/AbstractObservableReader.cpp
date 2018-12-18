#include "AbstractObservableReader.h"
#include "../event/ReaderEvent.h"
#include "../../transaction/SelectionRequest.h"
#include "../message/SeRequestSet.h"
#include "../message/SeResponseSet.h"
#include "../message/SeRequest.h"
#include "../message/SeResponse.h"
#include "../exception/KeypleReaderException.h"
#include "../exception/KeypleChannelStateException.h"
#include "../exception/KeypleIOReaderException.h"
#include "../SeReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                    using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                    using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                    using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> AbstractObservableReader::logger = org::slf4j::LoggerFactory::getLogger(AbstractObservableReader::typeid);

                    AbstractObservableReader::AbstractObservableReader(const std::string &pluginName, const std::string &readerName) : AbstractLoggedObservable<org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent>(readerName), pluginName(pluginName) {
                        this->before = System::nanoTime();
                    }

                    void AbstractObservableReader::startObservation() {
                    };

                    void AbstractObservableReader::stopObservation() {
                    };

                    void AbstractObservableReader::addObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) {
                        // if an observer is added to an empty list, start the observation
                        if (AbstractLoggedObservable<ReaderEvent>::countObservers() == 0) {
                            logger->debug("Start the reader monitoring.");
                            startObservation();
                        }
                        AbstractLoggedObservable<ReaderEvent>::addObserver(observer);
                    }

                    void AbstractObservableReader::removeObserver(std::shared_ptr<ObservableReader::ReaderObserver> observer) {
                        AbstractLoggedObservable<ReaderEvent>::removeObserver(observer);
                        if (AbstractLoggedObservable<ReaderEvent>::countObservers() == 0) {
                            logger->debug("Stop the reader monitoring.");
                            stopObservation();
                        }
                    }

                    std::shared_ptr<SeResponseSet> AbstractObservableReader::transmitSet(std::shared_ptr<SeRequestSet> requestSet) throw(KeypleReaderException) {
                        if (requestSet == nullptr) {
                            throw std::invalid_argument("seRequestSet must not be null");
                        }

                        std::shared_ptr<SeResponseSet> responseSet;

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            logger->debug("[{}] transmit => SEREQUESTSET = {}, elapsed {} ms.", this->getName(), requestSet->toString(), elapsedMs);
                        }

                        try {
                            responseSet = processSeRequestSet(requestSet);
                        }
                        catch (const KeypleChannelStateException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[{}] transmit => SEREQUESTSET channel failure. elapsed {}", elapsedMs);
                            /* Throw an exception with the responses collected so far. */
                            throw ex;
                        }
                        catch (const KeypleIOReaderException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[{}] transmit => SEREQUESTSET IO failure. elapsed {}", elapsedMs);
                            /* Throw an exception with the responses collected so far. */
                            throw ex;
                        }

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
                            this->before = timeStamp;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            logger->debug("[{}] transmit => SERESPONSESET = {}, elapsed {} ms.", this->getName(), responseSet->toString(), elapsedMs);
                        }

                        return responseSet;
                    }

                    std::shared_ptr<SeResponse> AbstractObservableReader::transmit(std::shared_ptr<SeRequest> seRequest) throw(KeypleReaderException) {
                        if (seRequest == nullptr) {
                            throw std::invalid_argument("seRequest must not be null");
                        }

                        std::shared_ptr<SeResponse> seResponse = nullptr;

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            logger->debug("[{}] transmit => SEREQUEST = {}, elapsed {} ms.", this->getName(), seRequest->toString(), elapsedMs);
                        }

                        try {
                            seResponse = processSeRequest(seRequest);
                        }
                        catch (const KeypleChannelStateException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[{}] transmit => SEREQUEST channel failure. elapsed {}", this->getName(), elapsedMs);
                            /* Throw an exception with the responses collected so far (ex.getSeResponse()). */
                            throw ex;
                        }
                        catch (const KeypleIOReaderException &ex) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - this->before) / 100000) / 10;
                            this->before = timeStamp;
                            logger->debug("[{}] transmit => SEREQUEST IO failure. elapsed {}", this->getName(), elapsedMs);
                            /* Throw an exception with the responses collected so far (ex.getSeResponse()). */
                            throw ex;
                        }

                        if (logger->isDebugEnabled()) {
                            long long timeStamp = System::nanoTime();
                            double elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
                            this->before = timeStamp;
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            logger->debug("[{}] transmit => SERESPONSE = {}, elapsed {} ms.", this->getName(), seResponse->toString(), elapsedMs);
                        }

                        return seResponse;
                    }

                    std::string AbstractObservableReader::getPluginName() {
                        return pluginName;
                    }

                    int AbstractObservableReader::compareTo(std::shared_ptr<SeReader> seReader) {
                        return this->getName().compare(seReader->getName());
                    }
                }
            }
        }
    }
}
