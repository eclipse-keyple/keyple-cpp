
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

#pragma once

#include <set>
#include <string>
#include "exceptionhelper.h"
#include <memory>
#include "Serializable.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeRequest; } } } } }

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {


                    /**
                     * Aggregates one or more SeRequests to a local or remote SE Reader, sent through a ProxyReader, in
                     * order to open a logical channel with a SE application to select, and to transfer a group of APDU
                     * commands to run.
                     *
                     * @see SeResponseSet
                     */
                    class SeRequestSet final : public std::enable_shared_from_this<SeRequestSet>, public Serializable {

                    public:
                        static constexpr long long serialVersionUID = 6255369841122636812LL;


                        /**
                         * List of requests. Each {@link SeRequest} will result in a {@link SeResponse} wrapped in a
                         * {@link SeResponseSet}.
                         */
                    private:
                        const std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> sortedRequests = std::make_shared<std::set<std::shared_ptr<SeRequest>>>();


                        /**
                         * Create an {@link SeRequestSet} from a list of {@link SeRequest}s.
                         * <ul>
                         * <li>A SeRequestSet could contain several SeRequest to manage the selection of different types
                         * of PO application.</li>
                         * <li>To exchange APDU commands with a specific selected PO application a single SeRequest is
                         * necessary.</li>
                         * </ul>
                         *
                         * @param seRequests List of {@link SeRequest}s
                         */
                    public:
                        SeRequestSet(std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> seRequests);

                        /**
                         * Create an {@link SeRequestSet} from a single {@link SeRequest}.
                         *
                         * @param seRequest single {@link SeRequest}
                         */
                        SeRequestSet(std::shared_ptr<SeRequest> seRequest);

                        /**
                         * Add an SeRequest to the current {@link SeRequestSet}
                         * 
                         * @param seRequest the {@link SeRequest} to add
                         */
                        void add(std::shared_ptr<SeRequest> seRequest);

                        /**
                         * List of requests
                         *
                         * @return Sorted list of requests
                         */
                        std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> getRequests();

                        /**
                         * Return the request when the list contains only one
                         *
                         * @return request
                         */
                        std::shared_ptr<SeRequest> getSingleRequest();

                        std::string toString() override;
                    };

                }
            }
        }
    }
}
