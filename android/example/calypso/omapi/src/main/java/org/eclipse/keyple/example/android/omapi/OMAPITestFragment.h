#pragma once

#include <string>
#include <vector>
#include <set>
#include <memory>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }

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
                namespace android {
                    namespace omapi {

                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using android::app::Fragment;
                        using android::os::Bundle;
                        using android::view::LayoutInflater;
                        using android::view::View;
                        using android::view::ViewGroup;
                        using android::widget::TextView;

                        /**
                         * View for OMAPI Tests
                         */
                        class OMAPITestFragment : public Fragment {


                        private:
                            static const std::string TAG;

                            std::shared_ptr<TextView> mText;

                        public:
                            static std::shared_ptr<OMAPITestFragment> newInstance();

                            /**
                             * Initialize SEProxy with Keyple Android OMAPI Plugin
                             * 
                             * @param savedInstanceState
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Override public void onCreate(@Nullable android.os.Bundle savedInstanceState)
                            void onCreate(std::shared_ptr<Bundle> savedInstanceState) override;

                            /**
                             * Initialize UI for this view
                             * 
                             * @param inflater
                             * @param container
                             * @param savedInstanceState
                             * @return
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Nullable @Override public android.view.View onCreateView(android.view.LayoutInflater inflater, @Nullable android.view.ViewGroup container, @Nullable android.os.Bundle savedInstanceState)
                            std::shared_ptr<View> onCreateView(std::shared_ptr<LayoutInflater> inflater, std::shared_ptr<ViewGroup> container, std::shared_ptr<Bundle> savedInstanceState) override;


                            /**
                             * Run a basic set of commands to connected Keyple readers
                             */
                            void onResume() override;

                            /**
                             * Run Hoplink Simple read command
                             */
                        private:
                            void runHoplinkSimpleRead(std::shared_ptr<SeReader> reader);



                            /**
                             * Revocation of the Activity
                             * from @{@link org.eclipse.keyple.plugin.android.omapi.AndroidOmapiReader} list of observers
                             */
                        public:
                            void onDestroy() override;



protected:
                            std::shared_ptr<OMAPITestFragment> shared_from_this() {
                                return std::static_pointer_cast<OMAPITestFragment>(android.app.Fragment::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
