#pragma once

#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SeSelection; } } } }
namespace org { namespace eclipse { namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { class ReadRecordsRespPars; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace transaction { class SelectionResponse; } } } }

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


                        using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using SelectionResponse = org::eclipse::keyple::transaction::SelectionResponse;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::app::Fragment;
                        using android::os::Bundle;
                        using android::view::LayoutInflater;
                        using android::view::View;
                        using android::view::ViewGroup;
                        using android::widget::TextView;


                        /**
                         * Test the Keyple NFC Plugin Configure the NFC reader Configure the Observability Run test commands
                         * when appropriate tag is detected.
                         */
                        class NFCTestFragment : public Fragment, public ObservableReader::ReaderObserver {

                        private:
                            static const std::shared_ptr<Logger> LOG;

                            static const std::string TAG;
                            static const std::string TAG_NFC_ANDROID_FRAGMENT;

                            // UI
                            std::shared_ptr<TextView> mText;

                            std::shared_ptr<SeReader> reader;
                            std::shared_ptr<SeSelection> seSelection;
                            std::shared_ptr<ReadRecordsRespPars> readEnvironmentParser;


                        public:
                            static std::shared_ptr<NFCTestFragment> newInstance();

                            /**
                             * Initialize SEProxy with Keyple Android NFC Plugin Add this view to the list of Observer
                             * of @{@link SeReader}
                             *
                             * @param savedInstanceState
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Override public void onCreate(@Nullable android.os.Bundle savedInstanceState)
                            void onCreate(std::shared_ptr<Bundle> savedInstanceState) override;


                            /**
                             * Initialize UI for NFC Test view
                             *
                             * @param inflater
                             * @param container
                             * @param savedInstanceState
                             * @return
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Nullable @Override public android.view.View onCreateView(android.view.LayoutInflater inflater, @Nullable android.view.ViewGroup container, @Nullable android.os.Bundle savedInstanceState)
                            std::shared_ptr<View> onCreateView(std::shared_ptr<LayoutInflater> inflater, std::shared_ptr<ViewGroup> container, std::shared_ptr<Bundle> savedInstanceState) override;

                        private:
                            class OnLongClickListenerAnonymousInnerClass : public View::OnLongClickListener {
                            private:
                                std::shared_ptr<NFCTestFragment> outerInstance;

                                std::shared_ptr<View> view;

                            public:
                                OnLongClickListenerAnonymousInnerClass(std::shared_ptr<NFCTestFragment> outerInstance, std::shared_ptr<View> view);

                                bool onLongClick(std::shared_ptr<View> view) override;

protected:
                                std::shared_ptr<OnLongClickListenerAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<OnLongClickListenerAnonymousInnerClass>(android.view.View.OnLongClickListener::shared_from_this());
                                }
                            };

                            /**
                             * Catch @{@link AndroidNfcReader} events When a SE is inserted, launch test commands
                             **
                             * @param event
                             */
                        public:
                            void update(std::shared_ptr<ReaderEvent> event_Renamed) override;


                            /**
                             * Run Calypso simple read transaction
                             * 
                             * @param defaultSelectionResponse
                             */
                        private:
                            void runCalyspoTransaction(std::shared_ptr<SelectionResponse> defaultSelectionResponse);



                            /**
                             * Revocation of the Activity from @{@link AndroidNfcReader} list of observers
                             */
                        public:
                            void onDestroy() override;


                            /**
                             * Initialize display
                             */
                        private:
                            void initTextView();

                            /**
                             * Append to tv a string containing an hex representation of the byte array provided in
                             * argument.
                             * <p>
                             * The font used is monospaced.
                             * 
                             * @param tv TextView
                             * @param ba byte array
                             */
                            static void appendHexBuffer(std::shared_ptr<TextView> tv, std::vector<char> &ba);

                            /**
                             * Append to tv a text colored according to the provided argument
                             * 
                             * @param tv TextView
                             * @param text string
                             * @param color color value
                             */
                            static void appendColoredText(std::shared_ptr<TextView> tv, const std::string &text, int color);

protected:
                            std::shared_ptr<NFCTestFragment> shared_from_this() {
                                return std::static_pointer_cast<NFCTestFragment>(android.app.Fragment::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
