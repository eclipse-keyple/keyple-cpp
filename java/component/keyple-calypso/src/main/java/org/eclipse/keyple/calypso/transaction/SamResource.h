#pragma once

#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/core/selection/SeResource.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace transaction { class CalypsoSam; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { class SeReader; } } } } }

/********************************************************************************
 * Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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
            namespace calypso {
                namespace transaction {

                    using SeResource = org::eclipse::keyple::core::selection::SeResource;
                    using SeReader = org::eclipse::keyple::core::seproxy::SeReader;

                    class SamResource : public SeResource<std::shared_ptr<CalypsoSam>> {
                        /**
                         * Constructor
                         *
                         * @param seReader the {@link SeReader} with which the SE is communicating
                         * @param calypsoSam the {@link CalypsoSam} information structure
                         */
                    public:
                        SamResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoSam> calypsoSam);

protected:
                        std::shared_ptr<SamResource> shared_from_this() {
                            return std::static_pointer_cast<SamResource>(org.eclipse.keyple.core.selection.SeResource<CalypsoSam>::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
