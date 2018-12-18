#pragma once

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
            namespace calypso {
                namespace command {
                    namespace sam {

                        /**
                         * This enumeration registers all revisions of SAM.
                         *
                         */
                        enum class SamRevision {

                            /** The revision of C1 and S1E SAM. 0x00, 0x80 */
                            C1,

                            /** The revision of S1D SAM SAM. 0x94 */
                            S1D
                        };

                    }
                }
            }
        }
    }
}
