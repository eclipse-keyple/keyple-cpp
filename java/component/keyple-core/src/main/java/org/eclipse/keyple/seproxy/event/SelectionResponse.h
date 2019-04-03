#pragma once

#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeResponseSet; } } } } }

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
                namespace event {

                using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;

                /**
                 * Class containing the {@link SeResponseSet} used from a default selection made at the
                 * {@link org.eclipse.keyple.seproxy.event.ObservableReader} level.
                 */
                class SelectionResponse : public std::enable_shared_from_this<SelectionResponse> {
                    /** The {@link SeResponseSet} */
                private:
                    const std::shared_ptr<SeResponseSet> selectionSeResponseSet;

                public:
                    SelectionResponse(std::shared_ptr<SeResponseSet> selectionSeResponseSet);

                    virtual std::shared_ptr<SeResponseSet> getSelectionSeResponseSet();

                    virtual ~SelectionResponse() { }
                };

                }
            }
        }
    }
}
