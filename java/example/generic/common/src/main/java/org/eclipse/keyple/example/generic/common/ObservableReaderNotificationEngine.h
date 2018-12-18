#pragma once

#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservablePlugin.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace example { namespace @generic { namespace common { class SpecificPluginObserver; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class PluginEvent; } } } } }

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
            namespace example {
                namespace generic_Renamed {
                    namespace common {

                        using ObservablePlugin = org::eclipse::keyple::seproxy::event_Renamed::ObservablePlugin;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;


                        class ObservableReaderNotificationEngine : public std::enable_shared_from_this<ObservableReaderNotificationEngine> {
                        private:
                            static const std::shared_ptr<Logger> logger;

                            std::shared_ptr<SpecificPluginObserver> pluginObserver;


                        public:
                            ObservableReaderNotificationEngine();

                            virtual void setPluginObserver();

                            /**
                             * This method is called whenever a Reader event occurs (SE insertion/removal)
                             */
                        public:
                            class SpecificReaderObserver : public std::enable_shared_from_this<SpecificReaderObserver>, public ObservableReader::ReaderObserver {
                                                private:
                                                    std::shared_ptr<ObservableReaderNotificationEngine> outerInstance;


                            public:
//JAVA TO C++ CONVERTER TODO TASK: No base class can be determined:
                                SpecificReaderObserver(std::shared_ptr<ObservableReaderNotificationEngine> outerInstance); //super();

                                virtual void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };

                            /**
                             * This method is called whenever a Plugin event occurs (reader insertion/removal)
                             */
                        public:
                            class SpecificPluginObserver : public std::enable_shared_from_this<SpecificPluginObserver>, public ObservablePlugin::PluginObserver {
                                                private:
                                                    std::shared_ptr<ObservableReaderNotificationEngine> outerInstance;


                            public:
                                std::shared_ptr<SpecificReaderObserver> readerObserver;

                                SpecificPluginObserver(std::shared_ptr<ObservableReaderNotificationEngine> outerInstance, std::shared_ptr<SpecificReaderObserver> readerObserver);

                                void update(std::shared_ptr<PluginEvent> event_Renamed) override;
                            };
                        };

                    }
                }
            }
        }
    }
}
