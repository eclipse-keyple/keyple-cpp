#pragma once

#include <vector>
#include <memory>
#include "Object.h"

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
                     * AnswerToReset bytes wrapper.
                     * <p>
                     * (May be enhanced to provide analysis methods)
                     */
                    class AnswerToReset : public std::enable_shared_from_this<AnswerToReset>, public Object{
                    private:
                        std::vector<char> atrBytes;

                    public:
                        AnswerToReset(std::vector<char> &atrBytes);

                        std::vector<char> getBytes();

                        bool equals(std::shared_ptr<void> o) override;

                        int hashCode() override;

                    protected:
                        void finalize() throw(std::runtime_error) override;
                    };

                }
            }
        }
    }
}
