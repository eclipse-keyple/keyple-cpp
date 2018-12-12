/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_EXCEPTION_READER_H
#define KEYPLE_SEPROXY_EXCEPTION_READER_H

#include "KeypleBaseException.hpp"

using namespace keyple::seproxy::message;

namespace keyple {
    namespace seproxy {
        namespace exception {

            class KeypleReaderException : public KeypleBaseException {
            public:
                /**
                 * New exception to be thrown
                 *
                 * @param message : message to identify the exception and the context
                 */
                KeypleReaderException(const std::string &message) : KeypleBaseException(message)
                {
                }

                /**
                 * Encapsulate a lower level reader exception
                 *
                 * @param message : message to add some context to the exception
                 * @param cause : lower level exception
                 */
                KeypleReaderException(std::string message, std::runtime_error cause) : KeypleBaseException(message, cause)
                {

                }

                /**
                 * New exception to be thrown
                 *
                 * @param message : message to identify the exception and the context
                 */
                 virtual ~KeypleReaderEXception()
                 {
                    delete seResponseSet;
                    delete seResponse;
                 }

                 /* Setters and Getters for SeResponseSet and SeResponse */
                 virtual SeResponseSet *getSeResponseSet()
                 {
                    return seResponseSet;
                 }

                 virtual void setSeResponseSet(SeResponseSet *seResponseSet)
                 {
                    this->seResponseSet = seResponseSet;
                 }

                 virtual SeResponse *getSeResponse()
                 {
                    return seResponse;
                 }

                 virtual void setSeResponse(SeResponse *seResponse)
                 {
                    this->seResponse = seResponse;
                 }

            private:
                /*
                 * SeResponseSet and SeResponse objects to carry partial responses in the event of a breakdown
                 * in communication with the SE.
                 */
                SeResponseSet *seResponseSet;
                seResponse *seResponse;
            };
        }
    }
}

#endif /* KEYPLE_SEPROXY_EXCEPTION_READER_H */
