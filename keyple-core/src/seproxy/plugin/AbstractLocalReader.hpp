/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_ABSTRACT_LOCAL_READER_H
#define KEYPLE_SEPROXY_ABSTRACT_LOCAL_READER_H

#include "AbstractObservableReader.hpp"
#include "SeResponseSet.hpp"

using namespace keyple::seproxy::message;
using namespace keyple::seproxy::plugin;

namespace keyple {
    namespace seproxy {
        namespace plugin {

            /**
             *
             */
            class AbstractLocalReader : public virtual AbstractObservableReader {
            public:
                /**
                 *
                 */
                AbstractLocalReader()
                : AbstractObservableReader("", "")
                {

                }

                /**
                 *
                 * @param pluginName
                 * @param readerName
                 */
                AbstractLocalReader(const std::string &pluginName, std::string &readerName)
                    : AbstractObservableReader(pluginName, readerName)
                {
                }

            protected:
                /**
                 * Do the transmission of all needed requestSet requests contained in the provided requestSet
                 * according to the protocol flag selection logic. The responseSet responses are returned in the
                 * responseSet object. The requestSet requests are ordered at application level and the
                 * responses match this order. When a requestSet is not matching the current PO, the responseSet
                 * responses pushed in the responseSet object is set to null.
                 *
                 * @param requestSet the request set
                 * @return SeResponseSet the response set
                 * @throws KeypleIOReaderException if a reader error occurs
                 */
                SeResponseSet* processSeRequestSet(SeRequestSet requestSet) override final
                {
//                    boolean requestMatchesProtocol[] = new boolean[requestSet.getRequests().size()];
//                    int requestIndex = 0, lastRequestIndex;
//
//                    // Determine which requests are matching the current ATR
//                    for (SeRequest request : requestSet.getRequests()) {
//                        requestMatchesProtocol[requestIndex] = protocolFlagMatches(request.getProtocolFlag());
//                        requestIndex++;
//                    }
//
//                    /*
//                     * we have now an array of booleans saying whether the corresponding request and the current
//                     * SE match or not
//                     */
//
//                    lastRequestIndex = requestIndex;
//                    requestIndex = 0;

                    /*
                     * The current requestSet is possibly made of several APDU command lists.
                     *
                     * If the requestMatchesProtocol is true we process the requestSet.
                     *
                     * If the requestMatchesProtocol is false we skip to the next requestSet.
                     *
                     * If keepChannelOpen is false, we close the physical channel for the last request.
                     */
                    std::list<SeResponse> responses; //= new ArrayList<SeResponse>();
//                    boolean stopProcess = false;
//                    for (SeRequest request : requestSet.getRequests()) {
//
//                        if (!stopProcess) {
//                            if (requestMatchesProtocol[requestIndex]) {
//                                logger.debug("[{}] processSeRequestSet => transmit {}", this.getName(),
//                                             request);
//                                SeResponse response = null;
//                                try {
//                                    response = processSeRequest(request);
//                                } catch (KeypleReaderException ex) {
//                                    /*
//                                     * The process has been interrupted. We are launching a
//                                     * KeypleReaderException with the responses collected so far.
//                                     */
//                                    /* Add the latest (and partial) SeResponse to the current list. */
//                                    responses.add(ex.getSeResponse());
//                                    /* Build a SeResponseSet with the available data. */
//                                    ex.setSeResponseSet(new SeResponseSet(responses));
//                                    throw ex;
//                                }
//                                responses.add(response);
//                                logger.debug("[{}] processSeRequestSet => receive {}", this.getName(),
//                                             response);
//                            } else {
//                                /*
//                                 * in case the protocolFlag of a SeRequest doesn't match the reader status, a
//                                 * null SeResponse is added to the SeResponseSet.
//                                 */
//                                responses.add(null);
//                            }
//                            requestIndex++;
//                            if (!request.isKeepChannelOpen()) {
//                                /*
//                                 * always explicitly close the logical channel to possibly process a multiple
//                                 * selection with the same AID
//                                 */
//                                closeLogicalChannel();
//                                if (lastRequestIndex == requestIndex) {
//                                    /*
//                                     * For the processing of the last SeRequest with a protocolFlag matching the
//                                     * SE reader status, if the logical channel doesn't require to be kept open,
//                                     * then the physical channel is closed.
//                                     */
//                                    closePhysicalChannel();
//
//                                    logger.debug("[{}] processSeRequestSet => Closing of the physical channel.",
//                                                 this.getName());
//                                }
//                            } else {
//                                if (isLogicalChannelOpen()) {
//                                    stopProcess = true;
//                                }
//                                /*
//                                 * When keepChannelOpen is true, we stop after the first matching request we
//                                 * exit the for loop here For the processing of a SeRequest with a protocolFlag
//                                 * which matches the current SE reader status, in case it's requested to keep
//                                 * the logical channel open, then the other remaining SeRequest are skipped, and
//                                 * null SeRequest are returned for them.
//                                 */
//                            }
//                        }
//                    }
                    return new SeResponseSet(responses);
                }
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_ABSTRACT_LOCAL_READER_H
