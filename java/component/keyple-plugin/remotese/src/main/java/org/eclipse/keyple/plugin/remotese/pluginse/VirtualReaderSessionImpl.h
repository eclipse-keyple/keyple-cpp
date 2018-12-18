#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/Observable.h"
#include "VirtualReaderSession.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class KeypleDto; } } } } } }

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
            namespace plugin {
                namespace remotese {
                    namespace pluginse {


                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                        using Observable = org::eclipse::keyple::util::Observable;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Manage RSE Reader Session
                         */
                        class VirtualReaderSessionImpl : public Observable<std::shared_ptr<KeypleDto>>, public VirtualReaderSession {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            const std::string sessionId;


                            // constructor
                        public:
                            VirtualReaderSessionImpl(const std::string &sessionId);


                            std::string getSessionId() override;



protected:
                            std::shared_ptr<VirtualReaderSessionImpl> shared_from_this() {
                                return std::static_pointer_cast<VirtualReaderSessionImpl>(org.eclipse.keyple.util.Observable<org.eclipse.keyple.plugin.remotese.transport.KeypleDto>::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
