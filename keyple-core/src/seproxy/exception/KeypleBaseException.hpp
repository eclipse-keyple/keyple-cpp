/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include <exception>

namespace keyple {
    namespace seproxy {
        namespace exception {
            class KeypleBaseException : public std::exception {
              public:
                /**
		 * New exception to be thrown
		 * 
		 * @param message : message to identify the exception and the context
		 */
                explicit KeypleBaseException(const std::string &message)
                {
                }

                /**
		 * Encapsulate a lower level exception (ie CardException, IOException, HostNotFoundException..)
		 * 
		 * @param message : message to identify the exception and the context
		 * @param cause : lower level exception
		 */
                /*explicit KeypleBaseException(std::string &message, Throwable cause)
                {
                }*/

		/**
		 * Destructor.
		 * 
		 * Virtual to allow for subclassing.
		 */
                virtual ~KeypleBaseException() throw()
                {
                }
            };
        } // namespace exception
    } // namespace seproxy
} // namespace keyple
