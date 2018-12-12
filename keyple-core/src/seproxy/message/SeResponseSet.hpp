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

#ifndef KEYPLE_SEPROXY_MSG_SERESPONSE_SET
#define KEYPLE_SEPROXY_MSG_SERESPONSE_SET

#include <vector>

#include "Export.hpp"
#include "SeResponse.hpp"

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
            class EXPORT SeResponseSet {
            public:
                /**
                 * Create an {@link SeResponseSet} from a single {@link SeResponse}
                 *
                 * @param response single {@link SeRequest}
                 */
                SeResponseSet(SeResponse *response);

                //
                //    /**
                //     * List of response seResponses
                //     *
                //     * @return List of response seResponses
                //     */
                //  public
                //    List<SeResponse> getResponses()
                //    {
                //        return seResponses;
                //    }
                //
                /**
                 * Create an {@link SeResponseSet} from a list of {@link SeResponse}s.
                 *
                 * @param seResponses List of seResponses
                 */
                SeResponseSet(std::vector<SeResponse*> &seResponses);

                /**
                 *
                 */
                ~SeResponseSet();
                //
                //
                //
                //    /**
                //     * Return the response when the list contains only one
                //     *
                //     * @return response
                //     */
                //  public
                //    SeResponse getSingleResponse()
                //    {
                //        if (seResponses.size() != 1)
                //        {
                //            throw new IllegalStateException("This method only support ONE element");
                //        }
                //        return seResponses.get(0);
                //    }
                //
                //    @Override public String toString()
                //    {
                //        return seResponses.size() + " SeReponse(s)";
                //    }

                /**
                 * List of response seResponses
                 *
                 * @return List of response seResponses
                 */
                std::vector<SeResponse*> getResponses();

            private:
                /**
                 * List of seResponses that were received following the transmission of the {@link SeRequest}.
                 */
                const std::vector<SeResponse*> seResponses;
            };
        } // namespace message
    } // namespace seproxy
} // namespace keyple

#endif /* KEYPLE_SEPROXY_MSG_SERESPONSE_SET */
