/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_NAMEABLE_CONFIGURABLE_H
#define KEYPLE_SEPROXY_NAMEABLE_CONFIGURABLE_H

#include <exception>
#include <map>
#include <stdexcept>
#include <string>

#include "KeypleBaseException.hpp"

namespace keyple {
    namespace util {
        class NameableConfigurable {
          public:
            /**
             * @return the ‘unique’ name of the item
             */
            virtual const std::string getName() = 0;

            /**
             * Gets the parameters
             *
             * @return the configuration of the item
             */
            virtual std::map<std::string, std::string> *getParameters() = 0;

            /**
             * allows to define a proprietary setting for a reader or a plugin (contactless protocols
             * polling sequence, baud rate, … etc.).
             *
             * @param key the parameter key
             * @param value the parameter value
             * @throws IllegalArgumentException if the parameter or the value is not supported
             * @throws KeypleBaseException if the parameter fails to be set up
             */
            virtual void setParameter(std::string key, std::string value) = 0; //throw(
            //std::illegal_argument, keyple::seproxy::exception::KeypleBaseException);

            /**
             * allows to define a set of proprietary settings for a reader or a plugin (contactless
             * protocols polling sequence, baud rate, … etc.).
             *
             * @param parameters Parameters to setup
             * @throws IllegalArgumentException if the parameters or the values is not supported
             * @throws KeypleBaseException if the parameter fails to be set up
             */
            virtual void setParameters(std::map<std::string, std::string> parameters) = 0; //throw(
            //std::illegal_argument, keyple::seproxy::exception::KeypleBaseException);
        };
    } // namespace util
} // namespace keyple

#endif
