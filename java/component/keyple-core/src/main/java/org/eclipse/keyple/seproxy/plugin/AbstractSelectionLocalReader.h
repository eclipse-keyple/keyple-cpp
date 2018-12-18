#pragma once

#include "../event/ObservableReader.h"
#include "AbstractLocalReader.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleApplicationSelectionException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleChannelStateException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleIOReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class Selector; } } } } }

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                    using KeypleApplicationSelectionException = org::eclipse::keyple::seproxy::exception::KeypleApplicationSelectionException;
                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                    using namespace org::eclipse::keyple::seproxy::message;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings({"PMD.ModifiedCyclomaticComplexity", "PMD.CyclomaticComplexity", "PMD.StdCyclomaticComplexity"}) public abstract class AbstractSelectionLocalReader extends AbstractLocalReader implements org.eclipse.keyple.seproxy.event.ObservableReader
                    class AbstractSelectionLocalReader : public AbstractLocalReader, public ObservableReader {
                    private:
                        static const std::shared_ptr<Logger> logger;

                    protected:
                        AbstractSelectionLocalReader(const std::string &pluginName, const std::string &readerName);

                        /**
                         * Gets the SE Answer to reset
                         *
                         * @return ATR returned by the SE or reconstructed by the reader (contactless)
                         */
                        virtual std::vector<char> getATR() = 0;

                        /**
                         * Tells if the physical channel is open or not
                         *
                         * @return true is the channel is open
                         */
                        virtual bool isPhysicalChannelOpen() = 0;

                        /**
                         * Attempts to open the physical channel
                         *
                         * @throws KeypleChannelStateException if the channel opening fails
                         */
                        virtual void openPhysicalChannel() = 0;

                        /**
                         * Opens a logical channel
                         * 
                         * @param selector the SE Selector: AID of the application to select or ATR regex
                         * @param successfulSelectionStatusCodes the list of successful status code for the select
                         *        command
                         * @return a {@link SelectionStatus} object containing the SE ATR, the SE FCI and a flag giving
                         *         the selection process result. When ATR or FCI are not available, they are set to null
                         * @throws KeypleChannelStateException - if a channel state exception occurred
                         * @throws KeypleIOReaderException - if an IO exception occurred
                         * @throws KeypleApplicationSelectionException - if the application selection is not successful
                         */
                        std::shared_ptr<SelectionStatus> openLogicalChannelAndSelect(std::shared_ptr<SeRequest::Selector> selector, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes) throw(KeypleChannelStateException, KeypleApplicationSelectionException, KeypleIOReaderException);

protected:
                        std::shared_ptr<AbstractSelectionLocalReader> shared_from_this() {
                            return std::static_pointer_cast<AbstractSelectionLocalReader>(AbstractLocalReader::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
