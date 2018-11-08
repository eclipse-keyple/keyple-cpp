/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_ABSTRACT_LOGGED_OBSERVABLE_H
#define KEYPLE_SEPROXY_ABSTRACT_LOGGED_OBSERVABLE_H

#include "NameableConfigurable.hpp"

using namespace keyple::util;

namespace keyple {
    namespace seproxy {
        namespace plugin {
            template <class T> class AbstractLoggedObservable : public NameableConfigurable {
              public:
                /**
                 * Item constructor Force the definition of a name through the use of super method.
                 *
                 * @param name name of the observed object
                 */
                AbstractLoggedObservable(std::string name) : name(name)
                {
                }

                /**
                 * Gets the reader name
                 *
                 * @return the reader name string
                 */
                virtual const std::string getName()
                {
                    return name;
                }

              private:
                /**
                 * The item name (must be unique)
                 */
                const std::string name;
            };
        } // namespace plugin
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_ABSTRACT_LOGGED_OBSERVABLE_H
