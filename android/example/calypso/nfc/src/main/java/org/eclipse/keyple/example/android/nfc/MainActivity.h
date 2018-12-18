#pragma once

#include <string>
#include <stdexcept>
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
            namespace example {
                namespace android {
                    namespace nfc {

                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::os::Bundle;
                        using android::support::v4::widget::DrawerLayout;
                        using android::support::v7::app::AppCompatActivity;
                        using android::view::MenuItem;


                        /**
                         * Example of @{@link SeProxyService} implementation based on the @{@link AndroidNfcPlugin}
                         *
                         */
                        class MainActivity : public AppCompatActivity {

                        private:
                            static const std::shared_ptr<Logger> LOG;

                            static const std::string TAG_NFC_TEST_VIEW;


                            std::shared_ptr<DrawerLayout> mDrawerLayout;


                            /**
                             * Setup navigation drawer
                             * 
                             * @param savedInstanceState
                             */
                        protected:
                            void onCreate(std::shared_ptr<Bundle> savedInstanceState) override;

                            /**
                             * Inject NFC Test View fragment into activity
                             */
                        private:
                            void activateNFCTestView();


                            /**
                             * Configure Navigation Drawer
                             */
                            void setupNavDrawer();

                        private:
                            class OnNavigationItemSelectedListenerAnonymousInnerClass : public NavigationView::OnNavigationItemSelectedListener {
                            private:
                                std::shared_ptr<MainActivity> outerInstance;

                            public:
                                OnNavigationItemSelectedListenerAnonymousInnerClass(std::shared_ptr<MainActivity> outerInstance);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Override public boolean onNavigationItemSelected(@NonNull android.view.MenuItem menuItem)
                                bool onNavigationItemSelected(std::shared_ptr<MenuItem> menuItem) override;

protected:
                                std::shared_ptr<OnNavigationItemSelectedListenerAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<OnNavigationItemSelectedListenerAnonymousInnerClass>(android.support.design.widget.NavigationView.OnNavigationItemSelectedListener::shared_from_this());
                                }
                            };

                            // 'Open' event for navigation drawer
                        public:
                            bool onOptionsItemSelected(std::shared_ptr<MenuItem> item) override;

protected:
                            std::shared_ptr<MainActivity> shared_from_this() {
                                return std::static_pointer_cast<MainActivity>(android.support.v7.app.AppCompatActivity::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
