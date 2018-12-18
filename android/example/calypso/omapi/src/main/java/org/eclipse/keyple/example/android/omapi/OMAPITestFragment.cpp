#include "OMAPITestFragment.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../../../../keyple-plugin/android-omapi/src/main/java/org/eclipse/keyple/plugin/android/omapi/AndroidOmapiPlugin.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/builder/ReadRecordsCmdBuild.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/builder/UpdateRecordCmdBuild.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactsProtocols.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace android {
                    namespace omapi {
                        using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                        using ReadRecordsCmdBuild = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                        using UpdateRecordCmdBuild = org::eclipse::keyple::calypso::command::po::builder::UpdateRecordCmdBuild;
                        using AndroidOmapiPlugin = org::eclipse::keyple::plugin::android::omapi::AndroidOmapiPlugin;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                        using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                        using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                        using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                        using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                        using ContactsProtocols = org::eclipse::keyple::seproxy::protocol::ContactsProtocols;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using android::app::Fragment;
                        using android::os::Bundle;
                        using android::support::annotation::Nullable;
                        using android::util::Log;
                        using android::view::LayoutInflater;
                        using android::view::View;
                        using android::view::ViewGroup;
                        using android::widget::TextView;
const std::string OMAPITestFragment::TAG = OMAPITestFragment::typeid->getSimpleName();

                        std::shared_ptr<OMAPITestFragment> OMAPITestFragment::newInstance() {
                            return std::make_shared<OMAPITestFragment>();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Override public void onCreate(@Nullable android.os.Bundle savedInstanceState)
                        void OMAPITestFragment::onCreate(std::shared_ptr<Bundle> savedInstanceState) {
                            Fragment::onCreate(savedInstanceState);

                            // initialize SEProxy with Android Plugin
                            Log::d(TAG, "Initialize SEProxy with Android OMAPI Plugin ");
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();
                            std::shared_ptr<SortedSet<std::shared_ptr<ReaderPlugin>>> plugins = std::set<std::shared_ptr<ReaderPlugin>>();
                            plugins->add(AndroidOmapiPlugin::getInstance());
                            seProxyService->setPlugins(plugins);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Nullable @Override public android.view.View onCreateView(android.view.LayoutInflater inflater, @Nullable android.view.ViewGroup container, @Nullable android.os.Bundle savedInstanceState)
                        std::shared_ptr<View> OMAPITestFragment::onCreateView(std::shared_ptr<LayoutInflater> inflater, std::shared_ptr<ViewGroup> container, std::shared_ptr<Bundle> savedInstanceState) {

                            std::shared_ptr<View> view = inflater->inflate(R::layout::fragment_omapi_test, container, false);
                            mText = std::static_pointer_cast<TextView>(view->findViewById(R::id::text));
                            return view;
                        }

                        void OMAPITestFragment::onResume() {
                            Fragment::onResume();

                            try {
//JAVA TO C++ CONVERTER TODO TASK: Java wildcard generics are not converted to C++:
//ORIGINAL LINE: java.util.SortedSet<? extends org.eclipse.keyple.seproxy.SeReader> readers = org.eclipse.keyple.seproxy.SeProxyService.getInstance().getPlugins().first().getReaders();
                                std::shared_ptr<SortedSet<? extends SeReader>> readers = SeProxyService::getInstance()->getPlugins()->first().getReaders();

                                if (readers == nullptr || readers->size() < 1) {
                                    mText->append("\nNo readers found in OMAPI Keyple Plugin");
                                    mText->append("\nTry to reload..");
                                }
                                else {
                                    for (auto aReader : readers) {
                                        Log::d(TAG, "Launching tests for reader : " + aReader->getName());
                                        runHoplinkSimpleRead(aReader);
                                    }

                                }

                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                            }

                        }

                        void OMAPITestFragment::runHoplinkSimpleRead(std::shared_ptr<SeReader> reader) {
                            Log::d(TAG, "Running HopLink Simple Read Tests");

                            getActivity().runOnUiThread([&] () {

                                mText->append("\nLaunching tests for reader : " + reader->getName());

                                try {



                                    std::string poAid = "A000000291A000000191";
                                    std::string t2UsageRecord1_dataFill = std::string("0102030405060708090A0B0C0D0E0F10") + "1112131415161718191A1B1C1D1E1F20"
                                                    + "2122232425262728292A2B2C2D2E2F30";

                                    mText->append("\n");
                                    mText->append("Selecting application : " + poAid);
                                    mText->append("\n");


                                    std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "Hoplink EF T2Environment");

                                    std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Usage = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x1A), static_cast<char>(0x01), true, static_cast<char>(0x30), "Hoplink EF T2Usage");

                                    std::shared_ptr<UpdateRecordCmdBuild> poUpdateRecordCmd_T2UsageFill = std::make_shared<UpdateRecordCmdBuild>(PoClass::ISO, static_cast<char>(0x1A), static_cast<char>(0x01), ByteArrayUtils::fromHex(t2UsageRecord1_dataFill), "Hoplink EF T2Usage");


                                    Boolean keepChannelOpen = false;

                                    mText->append("\n");
                                    mText->append("Executing command Calypso : " + poReadRecordCmd_T2Env->getName());
                                    mText->append("\n");
                                    mText->append("Executing command Calypso : " + poReadRecordCmd_T2Usage->getName());
                                    mText->append("\n");
                                    mText->append("Executing command Calypso : " + poUpdateRecordCmd_T2UsageFill->getName());
                                    mText->append("\n");
                                    mText->append("Keep Channel Open : " + StringHelper::toString(keepChannelOpen));
                                    mText->append("\n");
                                    mText->append("Using protocol : " + ContactsProtocols::PROTOCOL_ISO7816_3.getName());
                                    mText->append("\n ----\n ");

                                    std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                                    poApduRequestList = Arrays::asList(poReadRecordCmd_T2Env->getApduRequest(), poReadRecordCmd_T2Usage->getApduRequest(), poUpdateRecordCmd_T2UsageFill->getApduRequest());


                                    std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(std::make_shared<SeRequest::AidSelector>(ByteArrayUtils::fromHex(poAid)), poApduRequestList, ChannelState::CLOSE_AFTER, ContactsProtocols::PROTOCOL_ISO7816_3, nullptr);


                                    std::shared_ptr<SeResponseSet> seResponseSet = (std::static_pointer_cast<ProxyReader>(reader))->transmitSet(std::make_shared<SeRequestSet>(seRequest));


                                    mText->append("\n ---- \n");
                                    for (auto response : seResponseSet->getResponses()) {
                                        if (response != nullptr) {
                                            for (auto apdu : response->getApduResponses()) {
                                                mText->append("Response : " + std::to_string(apdu->getStatusCode()) + " - " + ByteArrayUtils::toHex(apdu->getBytes()));
                                                mText->append("\n");
                                            }
                                            mText->append("\n\n\n\n\n");

                                        }
                                        else {
                                            mText->append("Response : null");
                                            mText->append("\n\n\n\n\n");
                                        }
                                    }
                                }
//JAVA TO C++ CONVERTER WARNING: 'final' catch parameters are not available in C++:
//ORIGINAL LINE: catch (final org.eclipse.keyple.seproxy.exception.KeypleReaderException e)
                                catch (const KeypleReaderException &e) {
                                    getActivity().runOnUiThread([&] () {
                                        e->printStackTrace();
                                        mText->append("\n ---- \n");
                                        mText->append("IOReader Exception : " + e->what());

                                    }
                                   );

                                }
                            }
                           );
                        }

                        void OMAPITestFragment::onDestroy() {
                            Fragment::onDestroy();


                            /*
                             * // destroy AndroidNFC fragment if needed FragmentManager fm = getFragmentManager();
                             * Fragment f = fm.findFragmentByTag(TAG_OMAPI_ANDROID_FRAGMENT); if (f != null) {
                             * fm.beginTransaction().remove(f).commit(); }
                             */

                        }
                    }
                }
            }
        }
    }
}
