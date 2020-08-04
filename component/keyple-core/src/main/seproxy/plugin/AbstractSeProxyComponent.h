/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

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
 * This abstract class mutualizes the management of the name and part of the
 * configuration of SeProxy components (plugins and readers)
 */
class KEYPLECORE_API AbstractSeProxyComponent : virtual public ProxyElement {
public:
    /**
     * Constructor
     *
     * @param name the name of the component
     */
    AbstractSeProxyComponent(const std::string& name);

    /**
     *
     */
    virtual ~AbstractSeProxyComponent() = default;

    /**
     * @return the name of the component
     */
    virtual const std::string& getName() const override;

    /**
     * Sets at once a set of parameters for a reader
     * <p>
     * See {@link #setParameter(String, String)} for more details
     *
     * @param parameters a Map &lt;String, String&gt; parameter set
     * @throws KeypleBaseException if one of the parameters could not be set up
     */
    virtual void setParameters(
        const std::map<const std::string, const std::string>& parameters) final;

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
