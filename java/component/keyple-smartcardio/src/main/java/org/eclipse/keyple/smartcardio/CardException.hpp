/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_PLUGIN_PCSC_CARD_EXCEPTION_H
#define KEYPLE_PLUGIN_PCSC_CARD_EXCEPTION_H

#include <exception>

namespace keyple {
    namespace plugin {
        namespace pcsc {
            
            /*
             *
             */
            class CardException : public std::exception {
              public:
                /**
                 * New exception to be thrown
                 * 
                 * @param message : message to identify the exception and the context
                 */
                explicit CardException(const std::string &message)
                {
                }

                /**
                 * Destructor.
                 * 
                 * Virtual to allow for subclassing.
                 */
                virtual ~CardException() throw()
                {
                }
            };
        } // namespace pcsc
    } // namespace plugin
} // namespace keyple

#endif /* KEYPLE_PLUGIN_PCSC_CARD_EXCEPTION_H*/

