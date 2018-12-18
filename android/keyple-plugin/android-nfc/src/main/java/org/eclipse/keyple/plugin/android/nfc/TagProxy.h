#pragma once

#include <string>
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }

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

                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::nfc::Tag;
                        using android::nfc::tech::TagTechnology;

                        /**
                         * Proxy Tag for {@link IsoDep}, {@link MifareClassic}, {@link MifareUltralight} Invoke
                         * getTagTransceiver factory method to get a TagProxy object from a @{@link Tag} object
                         */
                        class TagProxy : public std::enable_shared_from_this<TagProxy>, public android::nfc->tech.TagTechnology {

                        private:
                            static const std::shared_ptr<Logger> LOG;


                            const std::shared_ptr<TagTechnology> tagTechnology;
                            const std::string tech;

                            TagProxy(std::shared_ptr<TagTechnology> tagTechnology, const std::string &tech);

                            /*
                             * Transceive
                             */
                        public:
                            virtual std::vector<char> transceive(std::vector<char> &data) throw(IOException);

                            virtual std::string getTech();

                            /**
                             * Create a TagProxy based on a {@link Tag}
                             *
                             * @param tag : tag to be proxied
                             * @return tagProxy
                             * @throws KeypleReaderException
                             */
                            static std::shared_ptr<TagProxy> getTagProxy(std::shared_ptr<Tag> tag) throw(KeypleReaderException);

                            /**
                             * Retrieve Answer to reset from Tag. For Isodep, getHiLayerResponse and getHiLayerResponse are
                             * used to retrieve ATR. For Mifare (Classic and UL) Smartcard, a virtual ATR is returned
                             * inspired by PS/SC standard 3B8F8001804F0CA000000306030001000000006A for Mifare Classic
                             * 3B8F8001804F0CA0000003060300030000000068 for Mifare Ultralight
                             *
                             * @return
                             */
                            virtual std::vector<char> getATR();

                            std::shared_ptr<Tag> getTag() override;

                            void connect() throw(IOException) override;

                            virtual ~TagProxy();

                            bool isConnected() override;
                        };

                    }
                }
            }
        }
    }
}
