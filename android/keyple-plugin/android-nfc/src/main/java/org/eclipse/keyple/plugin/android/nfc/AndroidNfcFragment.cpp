#include "AndroidNfcFragment.h"
#include "AndroidNfcPlugin.h"
#include "AndroidNfcReader.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::app::Fragment;
                        using android::content::Intent;
                        using android::nfc::NfcAdapter;
                        using android::os::Bundle;
                        using android::view::LayoutInflater;
                        using android::view::View;
                        using android::view::ViewGroup;
const std::shared_ptr<org::slf4j::Logger> AndroidNfcFragment::LOG = org::slf4j::LoggerFactory::getLogger(AndroidNfcFragment::typeid);

                        AndroidNfcFragment::AndroidNfcFragment() {
                            // Required empty public constructor
                        }

                        std::shared_ptr<AndroidNfcFragment> AndroidNfcFragment::newInstance() {
                            std::shared_ptr<AndroidNfcFragment> fragment = std::make_shared<AndroidNfcFragment>();
                            std::shared_ptr<Bundle> args = std::make_shared<Bundle>();
                            fragment->setArguments(args);
                            return fragment;
                        }

                        void AndroidNfcFragment::onCreate(std::shared_ptr<Bundle> savedInstanceState) {
                            Fragment::onCreate(savedInstanceState);
                            setRetainInstance(true); // Must be set to true
                            LOG->debug("onCreate");

                            std::shared_ptr<NfcAdapter> nfcAdapter = NfcAdapter::getDefaultAdapter(getActivity());

                            if (nfcAdapter == nullptr) {
                                LOG->warn("Your device does not support NFC");
                            }
                            else {
                                if (!nfcAdapter->isEnabled()) {
                                    LOG->warn("PLease enable NFC to communicate with NFC Elements");
                                }
                            }



                        }

                        std::shared_ptr<View> AndroidNfcFragment::onCreateView(std::shared_ptr<LayoutInflater> inflater, std::shared_ptr<ViewGroup> container, std::shared_ptr<Bundle> savedInstanceState) {
                            return nullptr;
                        }

                        void AndroidNfcFragment::onResume() {
                            Fragment::onResume();

                            // Process NFC intent i.e ACTION_TECH_DISCOVERED are processed by the reader. Many Intents
                            // can be received by the activity, only ACTION_TECH_DISCOVERED are processed
                            std::shared_ptr<Intent> intent = getActivity().getIntent();
                            LOG->debug("Intent type : " + intent->getAction());

                            // Enable Reader Mode for NFC Adapter
                            try {

                                if (intent->getAction() != nullptr && intent->getAction().equals(NfcAdapter::ACTION_TECH_DISCOVERED)) {
                                    LOG->debug("Handle ACTION TECH intent");

                                    (std::static_pointer_cast<AndroidNfcReader>(AndroidNfcPlugin::getInstance()->getReaders()->first()))->processIntent(intent);

                                }
                                else {
                                    LOG->debug("Intent is not of type ACTION TECH, do not process");

                                }

                                (std::static_pointer_cast<AndroidNfcReader>(SeProxyService::getInstance()->getPlugins()->first().getReaders().first()))->enableNFCReaderMode(getActivity());

                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                                LOG->error("KeypleReaders are not ready");
                            }



                        }

                        void AndroidNfcFragment::onPause() {
                            Fragment::onPause();
                            LOG->info("on Pause Fragment - Stopping Read Write Mode");

                            try {
                                // Disable Reader Mode for NFC Adapter
                                (std::static_pointer_cast<AndroidNfcReader>(SeProxyService::getInstance()->getPlugins()->first().getReaders().first()))->disableNFCReaderMode(getActivity());

                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                                LOG->error("NFC Reader is not ready");
                            }
                        }
                    }
                }
            }
        }
    }
}
