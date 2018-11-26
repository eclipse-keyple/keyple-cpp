/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License version 2.0 which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#ifndef KEYPLE_SEPROXY_MSG_APDUREQUEST_H
#define KEYPLE_SEPROXY_MSG_APDUREQUEST_H

#include <list>
#include <set>
#include <vector>
#include <string>

namespace keyple {
    namespace seproxy {
        namespace message {

            class ApduRequest {
                //static final
                //long serialVersionUID = 877369841119873812L;
            public:
                /**
                 * the constructor called by a ticketing application in order to build the APDU command requests
                 * to push to the ProxyReader.
                 *
                 * @param buffer Buffer of the APDU request
                 * @param case4 the case 4
                 * @param successfulStatusCodes the list of status codes to be considered as successful although
                 *        different from 9000
                 */
                ApduRequest(std::vector<uint8_t> buffer, bool case4,
                            std::set<int> successfulStatusCodes);

                /**
                 * Alternate constructor with name
                 *
                 * @param name name to be printed (e.g. in logs)
                 * @param buffer data buffer
                 * @param case4 case 4 flag (true if case 4)
                 * @param successfulStatusCodes the list of status codes to be considered as successful although
                 *        different from 9000
                 */
                ApduRequest(std::string &name, std::vector<uint8_t> buffer, bool case4,
                            std::set<int> successfulStatusCodes);

                /**
                 * Alternate constructor without status codes list
                 *
                 * @param buffer data buffer
                 * @param case4 case 4 flag (true if case 4)
                 */
                ApduRequest(std::vector<uint8_t> buffer, bool case4);

                /**
                 * Alternate constructor with name, without status codes list
                 *
                 * @param name name to be printed (e.g. in logs)
                 * @param buffer data buffer
                 * @param case4 case 4 flag (true if case 4)
                 */
                ApduRequest(std::string &name, std::vector<uint8_t> buffer, bool case4);

                /**
                 * Checks if is case 4.
                 *
                 * @return the case4 flag.
                 */
                bool isCase4();

                /**
                 * Name this APDU request
                 *
                 * @param name Name of the APDU request
                 */
                void setName(const std::string& name);

                /**
                 * Get the list of successful status codes for the request
                 *
                 * @return the list of status codes
                 */
                std::set<int> getSuccessfulStatusCodes();

                /**
                 * Get the name of this APDU request
                 *
                 * @return Name of the APDU request
                 */
                std::string& getName();

                /**
                 * Get the buffer of this APDU
                 *
                 * @return Name of the APDU request
                 */
                std::vector<uint8_t>& getBytes();

                /**
                 *
                 * @return
                 */
                std::string toString();

            private:
                /**
                 * Buffer of the APDU Request
                 */
                std::vector<uint8_t> bytes;

                /**
                 * a ‘case 4’ flag in order to explicitly specify, if it’s expected that the APDU command
                 * returns data → this flag is required to manage revision 2.4 Calypso Portable Objects and
                 * ‘S1Dx’ SAMs that presents a behaviour not compliant with ISO 7816-3 in contacts mode (not
                 * returning the 61XYh status).
                 */
                bool case4;

                /**
                 * List of status codes that should be considered successful although they are different from
                 * 9000
                 */
                std::set<int> successfulStatusCodes;

                /**
                 * Name of the request being sent
                 */
                std::string name;
            };
        } // namespace message
    } // namespace seproxy
} // namespace keyple

#endif // KEYPLE_SEPROXY_MSG_APDUREQUEST_H
