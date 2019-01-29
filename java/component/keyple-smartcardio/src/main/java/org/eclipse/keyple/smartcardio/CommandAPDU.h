/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_PLUGIN_PCSC_COMMAND_APDU_H
#define KEYPLE_PLUGIN_PCSC_COMMAND_APDU_H

#include <cstdint>
#include <vector>

namespace keyple {
    namespace plugin {
        namespace pcsc {

            /*
             *
             */
            class CommandAPDU {
              public:
                /**
                 * Constructor
                 *
                 * Constructs a CommandAPDU from a byte array containing the complete APDU
                 * contents (header and body).
                 *
                 * Note that the apdu bytes are copied to protect against subsequent
                 * modification.
                 *
                 * @param apdu the complete command APDU
                 * @throws NullPointerException if apdu is null
                 * @throws IllegalArgumentException if apdu does not contain a valid command APDU
                 */
                CommandAPDU(std::vector<char> &apdu) : apdu(apdu)
                {

                }

                /**
                 * Destructor
                 */
                ~CommandAPDU()
                {

                }

              private:
                /*
                 * Current APDU
                 */
                std::vector<char> apdu;

            };
        }
    }
}

#endif /* KEYPLE_PLUGIN_PCSC_RESPONSE_APDU_H */
