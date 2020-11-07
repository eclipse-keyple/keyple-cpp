/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

/* Core */
#include "AbstractSeProxyComponent.h"
#include "DefaultSelectionsRequest.h"
#include "KeypleCoreExport.h"
#include "KeypleReaderIOException.h"
#include "ObservableReader.h"
#include "ProxyReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::common;

/**
 * Abstract definition of an observable reader.
 * <ul>
 * <li>High level logging and benchmarking of SeRequestSet and SeRequest
 * transmission</li>
 * <li>Name-based comparison of ProxyReader (required for SortedSet&lt;
 * ProxyReader&gt;)</li>
 * <li>Plugin naming management</li>
 * </ul>
 */
class KEYPLECORE_API AbstractReader : public AbstractSeProxyComponent, public virtual ProxyReader {
public:
    /**
     *
     */
    using AbstractSeProxyComponent::getName;

    /**
     * Execute the transmission of a list of {@link SeRequest} and returns a list of
     * {@link SeResponse}
     * <p>
     * The {@link MultiSeRequestProcessing} parameter indicates whether all requests are to be sent
     * regardless of their result (PROCESS_ALL) or whether the process should stop at the first
     * request whose result is a success (FIRST_MATCH).
     * <p>
     * The {@link ChannelControl} parameter specifies whether the physical channel should be closed
     * (CLOSE_AFTER) or not (KEEP_OPEN) after all requests have been transmitted.
     * <p>
     * The global execution time (inter-exchange and communication) and the Set of SeRequest content
     * is logged (DEBUG level).
     * <p>
     * As the method is final, it cannot be extended.
     *
     * @param seRequests the request set
     * @param multiSeRequestProcessing the multi SE request processing mode
     * @param channelControl the channel control indicator
     * @return the response set
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    std::vector<std::shared_ptr<SeResponse>> transmitSeRequests(
        const std::vector<std::shared_ptr<SeRequest>>& seRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl) override;

    /**
     * Execute the transmission of a {@link SeRequest} and returns a {@link SeResponse}
     * <p>
     * The individual execution time (inter-exchange and communication) and the {@link SeRequest}
     * content is logged (DEBUG level).
     * <p>
     * As the method is final, it cannot be extended.
     *
     * @param seRequest the request to be transmitted
     * @param channelControl indicates if the channel has to be closed at the end of the processing
     * @return the received response
     * @throws KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    std::shared_ptr<SeResponse> transmitSeRequest(std::shared_ptr<SeRequest> seRequest,
                                                  const ChannelControl& channelControl) override;

    /**
     * Gets the name of plugin provided in the constructor.
     * <p>
     * The method will be used particularly for logging purposes. The plugin
     * name is also part of the ReaderEvent and PluginEvent objects.
     *
     * @return the plugin name String
     */
    const std::string& getPluginName() const;

protected:
    /**
     * The default DefaultSelectionsRequest to be executed upon SE insertion
     */
    std::shared_ptr<DefaultSelectionsRequest> mDefaultSelectionsRequest;

    /**
     * Indicate if all SE detected should be notified or only matching SE
     */
    ObservableReader::NotificationMode mNotificationMode;

    /**
     * Reader constructor
     * <p>
     * Force the definition of a name through the use of super method.
     * <p>
     * Initialize the time measurement
     *
     * @param pluginName the name of the plugin that instantiated the reader
     * @param readerName the name of the reader
     */
    AbstractReader(const std::string& pluginName, const std::string& name);

    /**
     *
     */
    virtual ~AbstractReader() = default;

    /**
     * Abstract method implemented by the AbstractLocalReader and VirtualReader
     * classes.
     * <p>
     * This method is handled by transmitSet.
     *
     * @param seRequests a {@link List} of {@link SeRequest} to be processed
     * @param multiSeRequestProcessing the multi se processing mode
     * @param channelControl indicates if the channel has to be closed at the
     *        end of the processing
     * @return the List of {@link SeResponse} (responses to the Set of {@link
     *         SeRequest})
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual std::vector<std::shared_ptr<SeResponse>> processSeRequests(
        const std::vector<std::shared_ptr<SeRequest>>& seRequests,
        const MultiSeRequestProcessing& multiSeRequestProcessing,
        const ChannelControl& channelControl) = 0;

    /**
     * Abstract method implemented by the AbstractLocalReader and VirtualReader
     * classes.
     * <p>
     * This method is handled by transmit.
     *
     * @param seRequest the {@link SeRequest} to be processed
     * @param channelControl a flag indicating if the channel has to be closed
     *        after the processing of the {@link SeRequest}
     * @return the {@link SeResponse} (responses to the {@link SeRequest})
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual std::shared_ptr<SeResponse> processSeRequest(
        const std::shared_ptr<SeRequest> seRequest,
        const ChannelControl& channelControl) = 0;

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(AbstractReader));

    /**
     * Timestamp recorder
     */
    long long mBefore = 0;

    /**
     *  Contains the name of the plugin
     */
    const std::string mPluginName;
};

}
}
}
}
