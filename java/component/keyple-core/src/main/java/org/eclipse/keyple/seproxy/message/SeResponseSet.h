#pragma once

#include <string>
#include <vector>
#include "exceptionhelper.h"
#include <memory>
#include "Serializable.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeResponse; } } } } }

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
            namespace seproxy {
                namespace message {


                    /**
                     * Aggregates the seResponses of a response from a local or remote SE Reader, received through a
                     * ProxyReader, including a group of APDU responses and the previous status of the logical channel
                     * with the targeted SE application.
                     *
                     * @see SeRequestSet
                     */
                    class SeResponseSet final : public std::enable_shared_from_this<SeResponseSet>, public Serializable {

                    public:
                        static constexpr long long serialVersionUID = 125369841119873812LL;


                        /**
                         * List of seResponses that were received following the transmission of the {@link SeRequest}.
                         */
                    private:
                        const std::vector<std::shared_ptr<SeResponse>> seResponses;

                        /**
                         * List of response seResponses
                         *
                         * @return List of response seResponses
                         */
                    public:
                        std::vector<std::shared_ptr<SeResponse>> getResponses();

                        /**
                         * Create an {@link SeResponseSet} from a list of {@link SeResponse}s.
                         *
                         * @param seResponses List of seResponses
                         */
                        SeResponseSet(std::vector<std::shared_ptr<SeResponse>> &seResponses);

                        /**
                         * Create an {@link SeResponseSet} from a single {@link SeResponse}
                         *
                         * @param response single {@link SeRequest}
                         */
                        SeResponseSet(std::shared_ptr<SeResponse> response);

                        /**
                         * Return the response when the list contains only one
                         *
                         * @return response
                         */
                        std::shared_ptr<SeResponse> getSingleResponse();

                        std::string toString() override;

                    public:
                      friend std::ostream &operator<<(std::ostream &os, SeResponseSet &s)
                      {
                         os << "<SeResponseSet: elmts: " << s.seResponses.size();

                         return os;
                      }
                    };

                }
            }
        }
    }
}
