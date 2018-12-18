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
            namespace plugin {
                namespace android {
                    namespace nfc {

                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::app::Fragment;
                        using android::os::Bundle;
                        using android::view::LayoutInflater;
                        using android::view::View;
                        using android::view::ViewGroup;

                        /**
                         * Add this {@link Fragment} to the Android application Activity {@link Fragment#onCreate(Bundle)}
                         * method to enable NFC
                         *
                         * getFragmentManager().beginTransaction() .add(AndroidNfcFragment.newInstance(),
                         * "myFragmentId").commit();
                         *
                         * By default the plugin only listens to events when your application activity is in the foreground.
                         * To activate NFC events while you application is not in the foreground, add the following
                         * statements to your activity definition in AndroidManifest.xml
                         *
                         * <intent-filter> <action android:name="android.nfc.action.TECH_DISCOVERED" /> </intent-filter>
                         * <meta-data android:name="android.nfc.action.TECH_DISCOVERED" android:resource="@xml/tech_list" />
                         *
                         * Create a xml/tech_list.xml file in your res folder with the following content <?xml version="1.0"
                         * encoding="utf-8"?> <resources xmlns:xliff="urn:oasis:names:tc:xliff:document:1.2"> <tech-list>
                         * <tech>android.nfc.tech.IsoDep</tech> <tech>android.nfc.tech.NfcA</tech> </tech-list> </resources>
                         */
                        class AndroidNfcFragment : public Fragment {

                        private:
                            static const std::shared_ptr<Logger> LOG;


                        public:
                            AndroidNfcFragment();

                            /**
                             * Use this factory method to create a new instance of this fragment using the provided
                             * parameters.
                             *
                             * @return A new instance of fragment NFCFragment.
                             */
                            static std::shared_ptr<AndroidNfcFragment> newInstance();


                            /**
                             * Checking of the NFC support on the Android device
                             */
                            void onCreate(std::shared_ptr<Bundle> savedInstanceState) override;

                            /**
                             * This fragment does not include UI
                             */
                            std::shared_ptr<View> onCreateView(std::shared_ptr<LayoutInflater> inflater, std::shared_ptr<ViewGroup> container, std::shared_ptr<Bundle> savedInstanceState) override;


                            /**
                             *
                             * Enable the Nfc Adapter in reader mode. While the fragment is active NFCAdapter will detect
                             * card of type @NFCAdapter.FLAG_READER_NFC_B Android Reader is called to process the
                             * communication with the ISO Card Fragment process Intent of ACTION_TECH_DISCOVERED if presents
                             */
                            void onResume() override;

                            void onPause() override;



protected:
                            std::shared_ptr<AndroidNfcFragment> shared_from_this() {
                                return std::static_pointer_cast<AndroidNfcFragment>(android.app.Fragment::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
