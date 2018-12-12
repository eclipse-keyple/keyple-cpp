/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_SEPROXY_MSG_SELECTION_STATUS_H
#define KEYPLE_SEPROXY_MSG_SELECTION_STATUS_H

#include "AnswerToReset.hpp"
#include "ApduResponse.hpp"

namespace keyple {
    namespace seproxy {
        namespace message {

            class SelectionStatus {
            public:
                virtual ~SelectionStatus()
                {
                    delete atr;
                    delete fci;
                }

                SelectionStatus(AnswerToReset *atr, ApduResponse *fci, bool hasMatched);
            private:
                /**
                 *
                 */
                AnswerToReset *const atr;

                /**
                 *
                 */
                ApduResponse *const fci;
            };
        }
    }
}

#endif /* KEYPLE_SEPROXY_MSG_SELECTION_STATUS_H */