#include "TagProxy.h"
#include "AndroidNfcProtocolSettings.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::nfc::Tag;
                        using android::nfc::tech::IsoDep;
                        using android::nfc::tech::MifareClassic;
                        using android::nfc::tech::MifareUltralight;
                        using android::nfc::tech::TagTechnology;
const std::shared_ptr<org::slf4j::Logger> TagProxy::LOG = org::slf4j::LoggerFactory::getLogger(TagProxy::typeid);

                        TagProxy::TagProxy(std::shared_ptr<TagTechnology> tagTechnology, const std::string &tech) : tagTechnology(tagTechnology), tech(tech) {
                        }

                        std::vector<char> TagProxy::transceive(std::vector<char> &data) throw(IOException) {

                            if (tech == AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_CLASSIC) {
                                return (std::static_pointer_cast<MifareClassic>(tagTechnology))->transceive(data);
                            }
                            else if (tech == AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_UL) {
                                return (std::static_pointer_cast<MifareUltralight>(tagTechnology))->transceive(data);
                            }
                            else if (tech == AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_ISODEP) {
                                return (std::static_pointer_cast<IsoDep>(tagTechnology))->transceive(data);
                            }
                            else {
                                return nullptr; // can not happen
                            }

                        }

                        std::string TagProxy::getTech() {
                            return tech;
                        }

                        std::shared_ptr<TagProxy> TagProxy::getTagProxy(std::shared_ptr<Tag> tag) throw(KeypleReaderException) {

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            LOG->info("Matching Tag Type : " + tag->toString());

                            if (Arrays::asList(tag->getTechList())->contains(AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_CLASSIC)) {
                                LOG->debug("Tag embedded into MifareClassic");
                                return std::make_shared<TagProxy>(android::nfc->tech.MifareClassic->get(tag), AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_CLASSIC);
                            }

                            if (Arrays::asList(tag->getTechList())->contains(AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_UL)) {
                                LOG->debug("Tag embedded into MifareUltralight");
                                return std::make_shared<TagProxy>(android::nfc->tech.MifareUltralight->get(tag), AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_UL);
                            }

                            if (Arrays::asList(tag->getTechList())->contains(AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_ISODEP)) {
                                LOG->debug("Tag embedded into IsoDep");
                                return std::make_shared<TagProxy>(android::nfc->tech.IsoDep->get(tag), AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_ISODEP);
                            }

                            throw std::make_shared<KeypleReaderException>("Keyple Android Reader supports only : " + AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_CLASSIC + ", " + AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_UL + ", " + AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_ISODEP);
                        }

                        std::vector<char> TagProxy::getATR() {

                            if (tech == AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_CLASSIC) {
                                return ByteArrayUtils::fromHex("3B8F8001804F0CA000000306030001000000006A");
                            }
                            else if (tech == AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_MIFARE_UL) {
                                return ByteArrayUtils::fromHex("3B8F8001804F0CA0000003060300030000000068");
                            }
                            else if (tech == AndroidNfcProtocolSettings::ProtocolSetting::NFC_TAG_TYPE_ISODEP) {
                                return (std::static_pointer_cast<IsoDep>(tagTechnology))->getHiLayerResponse() != nullptr ? (std::static_pointer_cast<IsoDep>(tagTechnology))->getHiLayerResponse() : (std::static_pointer_cast<IsoDep>(tagTechnology))->getHistoricalBytes();
                            }
                            else {
                                return nullptr; // can not happen
                            }

                        }

                        std::shared_ptr<Tag> TagProxy::getTag() {
                            return tagTechnology->getTag();
                        }

                        void TagProxy::connect() throw(IOException) {
                            tagTechnology->connect();
                        }

                        TagProxy::~TagProxy() {
                            tagTechnology->close();
                        }

                        bool TagProxy::isConnected() {
                            return tagTechnology->isConnected();
                        }
                    }
                }
            }
        }
    }
}
