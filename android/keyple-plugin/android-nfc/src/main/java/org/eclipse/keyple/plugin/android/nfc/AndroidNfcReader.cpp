#include "AndroidNfcReader.h"
#include "TagProxy.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleChannelStateException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleIOReaderException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                        using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using AbstractSelectionLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractSelectionLocalReader;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::app::Activity;
                        using android::content::Intent;
                        using android::nfc::NfcAdapter;
                        using android::nfc::Tag;
                        using android::os::Bundle;
const std::shared_ptr<org::slf4j::Logger> AndroidNfcReader::LOG = org::slf4j::LoggerFactory::getLogger(AndroidNfcReader::typeid);
const std::string AndroidNfcReader::READER_NAME = "AndroidNfcReader";
const std::string AndroidNfcReader::PLUGIN_NAME = "AndroidNfcPlugin";
const std::string AndroidNfcReader::FLAG_READER_SKIP_NDEF_CHECK = "FLAG_READER_SKIP_NDEF_CHECK";
const std::string AndroidNfcReader::FLAG_READER_NO_PLATFORM_SOUNDS = "FLAG_READER_NO_PLATFORM_SOUNDS";
const std::string AndroidNfcReader::FLAG_READER_PRESENCE_CHECK_DELAY = "FLAG_READER_PRESENCE_CHECK_DELAY";

                        AndroidNfcReader::AndroidNfcReader() : org::eclipse::keyple::seproxy::plugin::AbstractSelectionLocalReader(PLUGIN_NAME, READER_NAME) {
                            LOG->info("Init singleton NFC Reader");
                        }

const std::shared_ptr<AndroidNfcReader> AndroidNfcReader::SingletonHolder::instance = std::make_shared<AndroidNfcReader>();

                        std::shared_ptr<AndroidNfcReader> AndroidNfcReader::getInstance() {
                            return SingletonHolder::instance;
                        }

                        std::unordered_map<std::string, std::string> AndroidNfcReader::getParameters() {
                            return parameters;
                        }

                        void AndroidNfcReader::setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument) {
                            LOG->info("AndroidNfcReader supports the following parameters");
                            LOG->info(READER_NAME, "FLAG_READER_SKIP_NDEF_CHECK:\"int\", FLAG_READER_NO_PLATFORM_SOUNDS:\"int\", FLAG_READER_PRESENCE_CHECK_DELAY:\"int\"");

                            Boolean correctParameter = (key == AndroidNfcReader::FLAG_READER_SKIP_NDEF_CHECK && value == "1" || value == "0") || (key == AndroidNfcReader::FLAG_READER_NO_PLATFORM_SOUNDS && value == "1" || value == "0") || (key == AndroidNfcReader::FLAG_READER_PRESENCE_CHECK_DELAY && std::stoi(value) > -1);


                            if (correctParameter) {
                                LOG->warn("Adding parameter : " + key + " - " + value);
                                parameters.emplace(key, value);
                            }
                            else {
                                LOG->warn("Unrecognized parameter : " + key);
                                throw std::invalid_argument("Unrecognized parameter " + key + " : " + value);
                            }

                        }

                        TransmissionMode AndroidNfcReader::getTransmissionMode() {
                            return TransmissionMode::CONTACTLESS;
                        }

                        void AndroidNfcReader::onTagDiscovered(std::shared_ptr<Tag> tag) {
                            LOG->info("Received Tag Discovered event");
                            try {
                                tagProxy = TagProxy::getTagProxy(tag);
                                cardInserted();
                            }
                            catch (const KeypleReaderException &e) {
                                // print and do nothing
                                e->printStackTrace();
                                LOG->error(e->getLocalizedMessage());
                            }

                        }

                        bool AndroidNfcReader::isSePresent() {
                            return tagProxy != nullptr && tagProxy->isConnected();
                        }

                        std::vector<char> AndroidNfcReader::getATR() {
                            std::vector<char> atr = tagProxy->getATR();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            LOG->debug("ATR : " + Arrays->toString(atr));
                            return atr.size() > 0 && atr.size() > 0 ? atr : nullptr;
                        }

                        bool AndroidNfcReader::isPhysicalChannelOpen() {

                            return tagProxy != nullptr && tagProxy->isConnected();
                        }

                        void AndroidNfcReader::openPhysicalChannel() throw(KeypleChannelStateException) {

                            if (!isSePresent()) {
                                try {
                                    tagProxy->connect();
                                    LOG->info("Tag connected successfully : " + printTagId());

                                }
                                catch (const IOException &e) {
                                    LOG->error("Error while connecting to Tag ");
                                    e->printStackTrace();
                                    throw std::make_shared<KeypleChannelStateException>("Error while opening physical channel", e);
                                }
                            }
                            else {
                                LOG->info("Tag is already connected to : " + printTagId());
                            }
                        }

                        void AndroidNfcReader::closePhysicalChannel() throw(KeypleChannelStateException) {
                            try {
                                if (tagProxy != nullptr) {
                                    tagProxy->close();
                                    notifyObservers(std::make_shared<ReaderEvent>(PLUGIN_NAME, READER_NAME, ReaderEvent::EventType::SE_REMOVAL, nullptr));
                                    LOG->info("Disconnected tag : " + printTagId());
                                }
                            }
                            catch (const IOException &e) {
                                LOG->error("Disconnecting error");
                                throw std::make_shared<KeypleChannelStateException>("Error while closing physical channel", e);
                            }
                            tagProxy.reset();
                        }

                        std::vector<char> AndroidNfcReader::transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) {
                            // Initialization
                            LOG->debug("Data Length to be sent to tag : " + std::to_string(apduIn.size()));
                            LOG->debug("Data in : " + ByteArrayUtils::toHex(apduIn));
                            std::vector<char> data = apduIn;
                            std::vector<char> dataOut;
                            try {
                                dataOut = tagProxy->transceive(data);
                            }
                            catch (const IOException &e) {
                                e->printStackTrace();
                                throw std::make_shared<KeypleIOReaderException>("Error while transmitting APDU", e);
                            }
                            LOG->debug("Data out : " + ByteArrayUtils::toHex(dataOut));
                            return dataOut;
                        }

                        bool AndroidNfcReader::protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) {
                            return protocolsMap.find(protocolFlag) != protocolsMap.end() && protocolsMap[protocolFlag] == tagProxy->getTech();
                        }

                        void AndroidNfcReader::processIntent(std::shared_ptr<Intent> intent) {

                            // Extract Tag from Intent
                            std::shared_ptr<Tag> tag = intent->getParcelableExtra(NfcAdapter::EXTRA_TAG);
                            this->onTagDiscovered(tag);
                        }

                        std::string AndroidNfcReader::printTagId() {

                            if (tagProxy != nullptr && tagProxy->getTag() != nullptr) {
                                std::shared_ptr<StringBuilder> techList = std::make_shared<StringBuilder>();

                                // build a user friendly TechList
                                std::vector<std::string> techs = tagProxy->getTag()->getTechList();
                                for (int i = 0; i < techs.size(); i++) {
                                    // append a userfriendly Tech
                                    techList->append(StringHelper::replace(techs[i], "android.nfc.tech.", ""));
                                    // if not last tech, append separator
                                    if (i + 1 < techs.size()) {
                                        techList->append(", ");
                                    }
                                }



                                // build a hexa TechId
                                std::shared_ptr<StringBuilder> tagId = std::make_shared<StringBuilder>();
                                for (char b : tagProxy->getTag()->getId()) {
                                    tagId->append(std::string::format("%02X ", b));
                                }

                                return tagId + " - " + techList;
                            }
                            else {
                                return "no-tag";
                            }
                        }

                        int AndroidNfcReader::getFlags() {

                            int flags = 0;

                            std::string ndef = parameters[AndroidNfcReader::FLAG_READER_SKIP_NDEF_CHECK];
                            std::string nosounds = parameters[AndroidNfcReader::FLAG_READER_NO_PLATFORM_SOUNDS];

                            if (ndef != "" && ndef == "1") {
                                flags = flags | NfcAdapter::FLAG_READER_SKIP_NDEF_CHECK;
                            }

                            if (nosounds != "" && nosounds == "1") {
                                flags = flags | NfcAdapter::FLAG_READER_NO_PLATFORM_SOUNDS;
                            }

                            // Build flags list for reader mode
                            for (auto seProtocol : this->protocolsMap) {
                                if (ContactlessProtocols::PROTOCOL_ISO14443_4 == seProtocol->first) {
                                    flags = flags | NfcAdapter::FLAG_READER_NFC_B | NfcAdapter::FLAG_READER_NFC_A;

                                }
                                else if (seProtocol->first == ContactlessProtocols::PROTOCOL_MIFARE_UL || seProtocol->first == ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC) {
                                    flags = flags | NfcAdapter::FLAG_READER_NFC_A;
                                }
                            }

                            return flags;
                        }

                        std::shared_ptr<Bundle> AndroidNfcReader::getOptions() {
                            std::shared_ptr<Bundle> options = std::make_shared<Bundle>(1);
                            if (parameters.find(AndroidNfcReader::FLAG_READER_PRESENCE_CHECK_DELAY) != parameters.end()) {
                                Integer delay = std::stoi(parameters[AndroidNfcReader::FLAG_READER_PRESENCE_CHECK_DELAY]);
                                options->putInt(NfcAdapter::EXTRA_READER_PRESENCE_CHECK_DELAY, delay.intValue());
                            }
                            return options;
                        }

                        void AndroidNfcReader::enableNFCReaderMode(std::shared_ptr<Activity> activity) {
                            if (nfcAdapter == nullptr) {
                                nfcAdapter = NfcAdapter::getDefaultAdapter(activity);
                            }

                            int flags = getFlags();

                            std::shared_ptr<Bundle> options = getOptions();


//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            LOG->info("Enabling Read Write Mode with flags : " + std::to_string(flags) + " and options : " + options->toString());

                            // Reader mode for NFC reader allows to listen to NFC events without the Intent mechanism.
                            // It is active only when the activity thus the fragment is active.
                            nfcAdapter->enableReaderMode(activity, shared_from_this(), flags, options);
                        }

                        void AndroidNfcReader::disableNFCReaderMode(std::shared_ptr<Activity> activity) {
                            nfcAdapter->disableReaderMode(activity);

                        }
                    }
                }
            }
        }
    }
}
