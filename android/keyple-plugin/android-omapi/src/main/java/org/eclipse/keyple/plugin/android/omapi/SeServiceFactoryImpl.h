#pragma once

#include "ISeServiceFactory.h"
#include <string>
#include <vector>
#include <typeinfo>
#include "exceptionhelper.h"
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
            namespace plugin {
                namespace android {
                    namespace omapi {

                        using org::simalliance::openmobileapi::SEService;
                        using android::app::Application;
                        using android::content::Context;



                        class SeServiceFactoryImpl : public std::enable_shared_from_this<SeServiceFactoryImpl>, public ISeServiceFactory {

                        public:
                            static std::string TAG;

                            std::shared_ptr<SEService> connectToSe(std::shared_ptr<SEService::CallBack> callBack) override;

                            static Boolean isEnvironmentReady();

                            /**
                             * Retrieves Application Context automatically by a reflection invocation to method
                             * android.app.ActivityThread#currentApplication
                             * 
                             * @return App context
                             */
                        protected:
                            static std::shared_ptr<Application> getApplicationContext();

                        private:
                            static std::string getOMAPIVersion(std::shared_ptr<Context> context);
                        };

                    }
                }
            }
        }
    }
}
