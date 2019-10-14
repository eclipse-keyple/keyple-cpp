#pragma once

#include <memory>

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
            namespace integration {
                namespace calypso {

                    class EfData : public std::enable_shared_from_this<EfData> {

                    private:
                        int recNumb = 0;

                        char sfi = 0;

                        int recSize = 0;

                    public:
                        EfData(int recNumb, char sfi, int recSize);

                        virtual int getRecNumb();

                        virtual char getSfi();

                        virtual int getRecSize();
                    };

                }
            }
        }
    }
}
