#pragma once

#include <string>
#include <unordered_map>

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
            namespace util {

                using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;

                /**
                 * Allow {@link ProxyReader}s and {@link ReaderPlugin}s to receive configuration parameters.
                 */
                class Configurable {

                    /**
                     * Gets the parameters
                     *
                     * @return the configuration of the item
                     */
                public:
                    virtual std::unordered_map<std::string, std::string> getParameters() = 0;

                    /**
                     * allows to define a proprietary setting for a reader or a plugin (contactless protocols
                     * polling sequence, baud rate, … etc.).
                     *
                     * @param key the parameter key
                     * @param value the parameter value
                     * @throws IllegalArgumentException if the parameter or the value is not supported
                     * @throws KeypleBaseException if the parameter fails to be set up
                     */
                    virtual void setParameter(const std::string &key, const std::string &value) = 0;

                    /**
                     * allows to define a set of proprietary settings for a reader or a plugin (contactless
                     * protocols polling sequence, baud rate, … etc.).
                     *
                     * @param parameters Parameters to setup
                     * @throws IllegalArgumentException if the parameters or the values is not supported
                     * @throws KeypleBaseException if the parameter fails to be set up
                     */
                    virtual void setParameters(std::unordered_map<std::string, std::string> &parameters) = 0;
                };

            }
        }
    }
}
