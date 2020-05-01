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

#include "Configurable.h"
#include "Nameable.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::util;

/**
 * This abstract class mutualizes the management of the name and part of the
 * configuration of SeProxy components (plugins and readers)
 */
class AbstractSeProxyComponent
: public virtual Nameable, public virtual Configurable {
public:
    /**
     * Constructor
     *
     * @param name the name of the component
     */
    AbstractSeProxyComponent(const std::string& name);

    /**
     * @return the name of the component
     */
    virtual const std::string& getName() const final;

    /**
     * Sets at once a set of parameters for a reader
     * <p>
     * See {@link #setParameter(String, String)} for more details
     *
     * @param parameters a Map &lt;String, String&gt; parameter set
     * @throws KeypleBaseException if one of the parameters could not be set up
     */
    virtual void setParameters(
        const std::map<std::string, std::string>& parameters) final;

private:
    /**
     *
     */
    const std::string name;
};

}
}
}
