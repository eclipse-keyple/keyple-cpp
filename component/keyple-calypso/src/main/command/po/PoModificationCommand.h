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
                    namespace po {

                        /**
                         * Interface for commands that can modify the PO memory content
                         */
                        class PoModificationCommand {
                        public:
                            static const int MAGIC_VALUE = 12345678;
                            /*
                             * Magic number
                             */
                            const int magic = MAGIC_VALUE;
                        };

                    }
                }
            }
        }
    }
}
