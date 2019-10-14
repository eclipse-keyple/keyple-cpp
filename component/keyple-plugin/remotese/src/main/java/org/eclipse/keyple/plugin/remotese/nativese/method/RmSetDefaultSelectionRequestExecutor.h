#pragma once

#include "../../rm/RemoteMethodExecutor.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderServiceImpl; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class TransportDto; } } } } } } }

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
                    namespace nativese {
                        namespace method {

                            using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                            using RemoteMethodExecutor = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodExecutor;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

                            class RmSetDefaultSelectionRequestExecutor : public std::enable_shared_from_this<RmSetDefaultSelectionRequestExecutor>, public RemoteMethodExecutor {

                            private:
                                static const std::shared_ptr<Logger> logger;

                                const std::shared_ptr<NativeReaderServiceImpl> nativeReaderService;

                            public:
                                RmSetDefaultSelectionRequestExecutor(std::shared_ptr<NativeReaderServiceImpl> nativeReaderService);


                                std::shared_ptr<TransportDto> execute(std::shared_ptr<TransportDto> transportDto) override;
                            };

                        }
                    }
                }
            }
        }
    }
}
