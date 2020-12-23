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

#include <map>

/* Core */
#include "KeypleCoreExport.h"
#include "ProxyElement.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

/**
 * This abstract class mutualizes the management of the name and part of the configuration of
 * SeProxy components (plugins and readers)
 */
class KEYPLECORE_API AbstractSeProxyComponent {
public:
    /**
     * Constructor
     *
     * @param name the name of the component
     */
    explicit AbstractSeProxyComponent(const std::string& name);

    /**
     *
     */
    virtual ~AbstractSeProxyComponent() = default;

    /**
     * @return the name of the component
     */
    const std::string& getName() const;

    /**
     * Sets at once a set of parameters for a reader
     * <p>
     * See setParameter() for more details
     *
     * @param parameters a Map &lt;String, String&gt; parameter set
     * @throw KeypleBaseException if one of the parameters could not be set up
     */
    void setParameters(const std::map<const std::string, const std::string>& parameters);

    /**
     * This method has to be implemented by the reader plugin.<br>
     * Allows to define a proprietary setting for a reader or a plugin (contactless protocols
     * polling sequence, baud rate, … etc.).
     *
     * @param key the parameter key
     * @param value the parameter value
     * @throw IllegalArgumentException if the parameter or the value is not supported
     * @throw KeypleReaderIOException if the communication with the reader or the SE has failed
     */
    virtual void setParameter(const std::string& key, const std::string& value) = 0;

private:
    /**
     *
     */
    const std::string mName;
};

}
}
}
}
