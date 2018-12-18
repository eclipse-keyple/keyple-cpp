#pragma once

#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractSelectionLocalReader.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace android { namespace nfc { class TagProxy; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleChannelStateException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleIOReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocol; } } } } }

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


                        using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                        using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                        using AbstractSelectionLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractSelectionLocalReader;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::app::Activity;
                        using android::content::Intent;
                        using android::nfc::NfcAdapter;
                        using android::nfc::Tag;
                        using android::os::Bundle;


                        /**
                         * Implementation of {@link org.eclipse.keyple.seproxy.SeReader} to communicate with NFC Tag though
                         * Android {@link NfcAdapter}
                         *
                         * Configure NFCAdapter Protocols with {@link AndroidNfcReader#setParameter(String, String)}
                         *
                         *
                         */
                        class AndroidNfcReader final : public AbstractSelectionLocalReader, public NfcAdapter::ReaderCallback {

                        private:
                            static const std::shared_ptr<Logger> LOG;

                        public:
                            static const std::string READER_NAME;
                            static const std::string PLUGIN_NAME;


                            static const std::string FLAG_READER_SKIP_NDEF_CHECK;
                            static const std::string FLAG_READER_NO_PLATFORM_SOUNDS;
                            static const std::string FLAG_READER_PRESENCE_CHECK_DELAY;


                            // Android NFC Adapter
                        private:
                            std::shared_ptr<NfcAdapter> nfcAdapter;

                            // keep state between session if required
                            std::shared_ptr<TagProxy> tagProxy;

                            // flags for NFCAdapter
                            // private int flags = 0;
                            const std::unordered_map<std::string, std::string> parameters = std::unordered_map<std::string, std::string>();

                            /**
                             * Private constructor
                             */
                        public:
                            AndroidNfcReader();

                            /**
                             * Holder of singleton
                             */
                        private:
                            class SingletonHolder : public std::enable_shared_from_this<SingletonHolder> {
                            private:
                                static const std::shared_ptr<AndroidNfcReader> instance;
                            };

                            /**
                             * Access point for the unique instance of singleton
                             */
                            static std::shared_ptr<AndroidNfcReader> getInstance();

                            /**
                             * Get Reader parameters
                             *
                             * @return parameters
                             */
                        public:
                            std::unordered_map<std::string, std::string> getParameters();

                            /**
                             * Configure NFC Reader AndroidNfcReader supports the following parameters : FLAG_READER:
                             * SKIP_NDEF_CHECK (skip NDEF check when a smartcard is detected) FLAG_READER:
                             * NO_PLATFORM_SOUNDS (disable device sound when nfc smartcard is detected)
                             * EXTRA_READER_PRESENCE_CHECK_DELAY: "Int" (see @NfcAdapter.EXTRA_READER_PRESENCE_CHECK_DELAY)
                             *
                             * @param key the parameter key
                             * @param value the parameter value
                             * @throws IOException
                             */
                            void setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument) override;

                            /**
                             * The transmission mode is always CONTACTLESS in a NFC reader
                             * 
                             * @return the current transmission mode
                             */
                            TransmissionMode getTransmissionMode() override;


                            /**
                             * Callback function invoked by @{@link NfcAdapter} when a @{@link Tag} is discovered A
                             * TagTransceiver is created based on the Tag technology see {@link TagProxy#getTagProxy(Tag)}
                             * Do not call this function directly.
                             *
                             * @param tag : detected tag
                             */
                            void onTagDiscovered(std::shared_ptr<Tag> tag) override;

                            bool isSePresent() override;

                        protected:
                            std::vector<char> getATR() override;

                            bool isPhysicalChannelOpen() override;

                            void openPhysicalChannel() throw(KeypleChannelStateException) override;

                            void closePhysicalChannel() throw(KeypleChannelStateException) override;


                            std::vector<char> transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) override;


                            bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) override;

                            /**
                             * Process data from NFC Intent
                             *
                             * @param intent : Intent received and filterByProtocol by xml tech_list
                             */
                        public:
                            void processIntent(std::shared_ptr<Intent> intent);

                            std::string printTagId();



                            /**
                             * Build Reader Mode flags Integer from parameters
                             * 
                             * @return flags Integer
                             */
                            int getFlags();

                            /**
                             * Build Reader Mode options Bundle from parameters
                             * 
                             * @return options
                             */
                            std::shared_ptr<Bundle> getOptions();

                            void enableNFCReaderMode(std::shared_ptr<Activity> activity);

                            void disableNFCReaderMode(std::shared_ptr<Activity> activity);

protected:
                            std::shared_ptr<AndroidNfcReader> shared_from_this() {
                                return std::static_pointer_cast<AndroidNfcReader>(org.eclipse.keyple.seproxy.plugin.AbstractSelectionLocalReader::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
