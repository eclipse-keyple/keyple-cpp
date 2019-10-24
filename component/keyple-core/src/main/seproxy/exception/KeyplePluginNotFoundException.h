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

#pragma once

#include <string>
#include <memory>

#include "KeyplePluginException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace exception {

/**
 * Exception thrown when {@link org.eclipse.keyple.seproxy.ReaderPlugin} is not found
 */
class KeyplePluginNotFoundException : public KeyplePluginException {
public:
    /**
     * Exception thrown when {@link org.eclipse.keyple.seproxy.ReaderPlugin} is not found
     *
     * @param pluginName : pluginName that has not been found
     */
    KeyplePluginNotFoundException(const std::string &pluginName);

protected:
    /**
     *
     */
    std::shared_ptr<KeyplePluginNotFoundException> shared_from_this()
    {
        return std::static_pointer_cast<KeyplePluginNotFoundException>(KeyplePluginException::shared_from_this());
    }
};

}
}
}
}
