#pragma once

#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractStaticReader.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class NoStackTraceThrowable; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleApplicationSelectionException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleChannelStateException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class Selector; } } } } }
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
                    namespace omapi {

                        using KeypleApplicationSelectionException = org::eclipse::keyple::seproxy::exception::KeypleApplicationSelectionException;
                        using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                        using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using namespace org::eclipse::keyple::seproxy::message;
                        using AbstractStaticReader = org::eclipse::keyple::seproxy::plugin::AbstractStaticReader;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                        using org::simalliance::openmobileapi::Channel;
                        using org::simalliance::openmobileapi::Reader;

                        /**
                         * Communicates with Android readers throught the Open Mobile API see {@link Reader} Instances of
                         * this class represent SE readers supported by this device. These readers can be physical devices
                         * or virtual devices. They can be removable or not. They can contain one SE that can or cannot be
                         * removed.
                         */
                        class AndroidOmapiReader final : public AbstractStaticReader {


                        private:
                            static const std::string TAG;

                            std::shared_ptr<Reader> omapiReader;
                            std::shared_ptr<Channel> openChannel = nullptr;
                            std::vector<char> openApplication;
                            std::unordered_map<std::string, std::string> parameters = std::unordered_map<std::string, std::string>();

                        protected:
                            AndroidOmapiReader(const std::string &pluginName, std::shared_ptr<Reader> omapiReader, const std::string &readerName);


                        public:
                            std::unordered_map<std::string, std::string> getParameters() override;

                            void setParameter(const std::string &key, const std::string &value) override;

                            /**
                             * The transmission mode is always CONTACTS in an OMAPI reader
                             *
                             * @return the current transmission mode
                             */
                            TransmissionMode getTransmissionMode() override;

                            /**
                             * Check if a SE is present in this reader. see {@link Reader#isSecureElementPresent()}
                             * 
                             * @return True if the SE is present, false otherwise
                             * @throws KeypleReaderException
                             */
                            bool isSePresent() throw(NoStackTraceThrowable) override;

                            /**
                             * Open a Channel to the application AID if not open yet. see {@link Reader#openSession()} see
                             * {@link Session#openLogicalChannel(byte[])}
                             * 
                             * @param selector: AID of the application to select
                             * @return Array : index[0] : ATR and index[1] :FCI
                             * @throws KeypleReaderException
                             */
                        protected:
                            std::shared_ptr<SelectionStatus> openLogicalChannelAndSelect(std::shared_ptr<SeRequest::Selector> selector, std::shared_ptr<Set<Integer>> successfulSelectionStatusCodes) throw(KeypleChannelStateException, KeypleApplicationSelectionException) override;

                            /**
                             * Close session see {@link Session#close()}
                             * 
                             * @throws KeypleReaderException
                             */
                            void closePhysicalChannel() override;

                            /**
                             * Transmit an APDU command (as per ISO/IEC 7816) to the SE see {@link Channel#transmit(byte[])}
                             * 
                             * @param apduIn byte buffer containing the ingoing data
                             * @return
                             * @throws KeypleReaderException
                             */
                            std::vector<char> transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) override;

                            /**
                             * The only protocol Fla
                             * 
                             * @param protocolFlag
                             * @return true
                             * @throws KeypleReaderException
                             */
                            bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) override;

protected:
                            std::shared_ptr<AndroidOmapiReader> shared_from_this() {
                                return std::static_pointer_cast<AndroidOmapiReader>(org.eclipse.keyple.seproxy.plugin.AbstractStaticReader::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
