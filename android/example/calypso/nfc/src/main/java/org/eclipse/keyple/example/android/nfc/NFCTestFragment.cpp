#include "NFCTestFragment.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../../../../keyple-plugin/android-nfc/src/main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcPlugin.h"
#include "../../../../../../../../../../../../keyple-plugin/android-nfc/src/main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcFragment.h"
#include "../../../../../../../../../../../../keyple-plugin/android-nfc/src/main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcProtocolSettings.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "CalypsoClassicInfo.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SelectionResponse.h"
#include "../../../../../../../../../../../../keyple-plugin/android-nfc/src/main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcReader.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include "../../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace android {
                    namespace nfc {
                        using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                        using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                        using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                        using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                        using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                        using AndroidNfcFragment = org::eclipse::keyple::plugin::android::nfc::AndroidNfcFragment;
                        using AndroidNfcPlugin = org::eclipse::keyple::plugin::android::nfc::AndroidNfcPlugin;
                        using AndroidNfcProtocolSettings = org::eclipse::keyple::plugin::android::nfc::AndroidNfcProtocolSettings;
                        using AndroidNfcReader = org::eclipse::keyple::plugin::android::nfc::AndroidNfcReader;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                        using SelectionResponse = org::eclipse::keyple::transaction::SelectionResponse;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::app::Fragment;
                        using android::app::FragmentManager;
                        using android::graphics::Color;
                        using android::os::Bundle;
                        using android::support::annotation::Nullable;
                        using android::text::Spannable;
                        using android::text::style::ForegroundColorSpan;
                        using android::text::style::RelativeSizeSpan;
                        using android::text::style::TypefaceSpan;
                        using android::view::LayoutInflater;
                        using android::view::View;
                        using android::view::ViewGroup;
                        using android::widget::TextView;
const std::shared_ptr<org::slf4j::Logger> NFCTestFragment::LOG = org::slf4j::LoggerFactory::getLogger(NFCTestFragment::typeid);
const std::string NFCTestFragment::TAG = NFCTestFragment::typeid->getSimpleName();
const std::string NFCTestFragment::TAG_NFC_ANDROID_FRAGMENT = "org.eclipse.keyple.plugin.android.nfc.AndroidNfcFragment";

                        std::shared_ptr<NFCTestFragment> NFCTestFragment::newInstance() {
                            return std::make_shared<NFCTestFragment>();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Override public void onCreate(@Nullable android.os.Bundle savedInstanceState)
                        void NFCTestFragment::onCreate(std::shared_ptr<Bundle> savedInstanceState) {
                            Fragment::onCreate(savedInstanceState);

                            // 1 - First initialize SEProxy with Android Plugin
                            LOG->debug("Initialize SEProxy with Android Plugin");
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();
                            std::shared_ptr<SortedSet<std::shared_ptr<ReaderPlugin>>> plugins = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();
                            plugins->add(AndroidNfcPlugin::getInstance());
                            seProxyService->setPlugins(plugins);

                            // 2 - add NFC Fragment to activity in order to communicate with Android Plugin
                            LOG->debug(std::string("Add Keyple NFC Fragment to activity in order to ") + "communicate with Android Plugin");
                            getFragmentManager().beginTransaction()->add(AndroidNfcFragment::newInstance(), TAG_NFC_ANDROID_FRAGMENT).commit();

                            try {
                                // define task as an observer for ReaderEvents
                                LOG->debug("Define this view as an observer for ReaderEvents");
                                reader = seProxyService->getPlugins()->first().getReaders().first();
                                /* remove the observer if it already exist */
                                (std::static_pointer_cast<ObservableReader>(reader))->addObserver(shared_from_this());

                                reader->setParameter("FLAG_READER_PRESENCE_CHECK_DELAY", "100");
                                reader->setParameter("FLAG_READER_NO_PLATFORM_SOUNDS", "0");
                                reader->setParameter("FLAG_READER_SKIP_NDEF_CHECK", "0");


                                // with this protocol settings we activate the nfc for ISO1443_4 protocol
                                (std::static_pointer_cast<ObservableReader>(reader))->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));

                                /*
                                 * Prepare a Calypso PO selection
                                 */
                                seSelection = std::make_shared<SeSelection>(reader);

                                /*
                                 * Setting of an AID based selection of a Calypso REV3 PO
                                 *
                                 * Select the first application matching the selection AID whatever the SE communication
                                 * protocol keep the logical channel open after the selection
                                 */

                                /*
                                 * Calypso selection: configures a PoSelector with all the desired attributes to make
                                 * the selection and read additional information afterwards
                                 */
                                std::shared_ptr<PoSelector> poSelector = std::make_shared<PoSelector>(ByteArrayUtils::fromHex(CalypsoClassicInfo::AID), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "AID: " + CalypsoClassicInfo::AID);

                                /*
                                 * Prepare the reading order and keep the associated parser for later use once the
                                 * selection has been made.
                                 */
                                readEnvironmentParser = poSelector->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EnvironmentAndHolder, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EnvironmentAndHolder (SFI=%02X))", CalypsoClassicInfo::SFI_EnvironmentAndHolder));

                                /*
                                 * Add the selection case to the current selection (we could have added other cases
                                 * here)
                                 */
                                seSelection->prepareSelection(poSelector);

                                /*
                                 * Provide the SeReader with the selection operation to be processed when a PO is
                                 * inserted.
                                 */
                                (std::static_pointer_cast<ObservableReader>(reader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);

                                /*
                                 * uncomment to active protocol listening for Mifare ultralight ((AndroidNfcReader)
                                 *
                                 * reader).addSeProtocolSetting( AndroidNfcProtocolSettings.SETTING_PROTOCOL_MIFARE_UL);
                                 *
                                 * uncomment to active protocol listening for Mifare Classic ((AndroidNfcReader)
                                 * reader).addSeProtocolSetting(
                                 * AndroidNfcProtocolSettings.SETTING_PROTOCOL_MIFARE_CLASSIC);
                                 */

                            }
                            catch (const KeypleBaseException &e) {
                                e->printStackTrace();
                            }
                            catch (const std::invalid_argument &e) {
                                e.printStackTrace();
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Nullable @Override public android.view.View onCreateView(android.view.LayoutInflater inflater, @Nullable android.view.ViewGroup container, @Nullable android.os.Bundle savedInstanceState)
                        std::shared_ptr<View> NFCTestFragment::onCreateView(std::shared_ptr<LayoutInflater> inflater, std::shared_ptr<ViewGroup> container, std::shared_ptr<Bundle> savedInstanceState) {

                            // Define UI components
                            std::shared_ptr<View> view = inflater->inflate(org::eclipse::keyple::example::android::nfc::R::layout::fragment_nfc_test, container, false);
                            mText = view->findViewById(org::eclipse::keyple::example::android::nfc::R::id::text);
                            mText->setOnLongClickListener(std::make_shared<OnLongClickListenerAnonymousInnerClass>(shared_from_this(), view));
                            initTextView();
                            return view;
                        }

                        NFCTestFragment::OnLongClickListenerAnonymousInnerClass::OnLongClickListenerAnonymousInnerClass(std::shared_ptr<NFCTestFragment> outerInstance, std::shared_ptr<View> view) {
                            this->outerInstance = outerInstance;
                            this->view = view;
                        }

                        bool NFCTestFragment::OnLongClickListenerAnonymousInnerClass::onLongClick(std::shared_ptr<View> view) {
                            outerInstance->initTextView();
                            return true;
                        }

                        void NFCTestFragment::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            getActivity().runOnUiThread([&] () {

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                LOG->info("New ReaderEvent received : " + event_Renamed->toString());

                                switch (event_Renamed->getEventType()) {
                                    case SE_MATCHED:
                                        runCalyspoTransaction(event_Renamed->getDefaultSelectionResponse());
                                        break;

                                    case SE_INSERTED:

                                        // execute simple tests
                                        break;

                                    case SE_REMOVAL:
                                        // mText.append("\n ---- \n");
                                        // mText.append("Connection closed to tag");
                                        break;

                                    case IO_ERROR:
                                        mText->append("\n ---- \n");
                                        mText->setText("Error reading card");
                                        break;

                                }
                            }
                           );
                        }

                        void NFCTestFragment::runCalyspoTransaction(std::shared_ptr<SelectionResponse> defaultSelectionResponse) {
                            LOG->debug("Running Calypso Simple Read transaction");
                            getActivity().runOnUiThread([&] () {
                                try {
                                    initTextView();

                                    /*
                                     * print tag info in View
                                     */
                                    mText->append("\n ---- \n");
                                    mText->append((std::static_pointer_cast<AndroidNfcReader>(reader))->printTagId());
                                    mText->append("\n ---- \n");
                                    if (seSelection->processDefaultSelection(defaultSelectionResponse)) {
                                        std::shared_ptr<CalypsoPo> calypsoPo = std::static_pointer_cast<CalypsoPo>(seSelection->getSelectedSe());

                                        mText->append("\nCalypso PO selection: ");
                                        appendColoredText(mText, "SUCCESS\n", Color::GREEN);
                                        mText->append("AID: ");
                                        appendHexBuffer(mText, ByteArrayUtils::fromHex(CalypsoClassicInfo::AID));

                                        /*
                                         * Retrieve the data read from the parser updated during the selection
                                         * process
                                         */
                                        std::vector<char> environmentAndHolder = (readEnvironmentParser->getRecords())->get(static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1));

                                        mText->append("\n\nEnvironment and Holder file: ");
                                        appendHexBuffer(mText, environmentAndHolder);

                                        appendColoredText(mText, "\n\n2nd PO exchange:\n", Color::BLACK);
                                        mText->append("* read the event log file");
                                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(reader, calypsoPo);

                                        /*
                                         * Prepare the reading order and keep the associated parser for later use
                                         * once the transaction has been processed.
                                         */
                                        std::shared_ptr<ReadRecordsRespPars> readEventLogParser = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EventLog (SFI=%02X, recnbr=%d))", CalypsoClassicInfo::SFI_EventLog, CalypsoClassicInfo::RECORD_NUMBER_1));

                                        /*
                                         * Actual PO communication: send the prepared read order, then close the
                                         * channel with the PO
                                         */
                                        if (poTransaction->processPoCommands(ChannelState::CLOSE_AFTER)) {
                                            mText->append("\nTransaction: ");
                                            appendColoredText(mText, "SUCCESS\n", Color::GREEN);

                                            /*
                                             * Retrieve the data read from the parser updated during the transaction
                                             * process
                                             */
                                            std::vector<char> eventLog = (readEventLogParser->getRecords())->get(static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1));

                                            /* Log the result */
                                            mText->append("\nEventLog file:\n");
                                            appendHexBuffer(mText, eventLog);
                                        }
                                        appendColoredText(mText, "\n\nEnd of the Calypso PO processing.", Color::BLACK);
                                    }
                                    else {
                                        appendColoredText(mText, "The selection of the PO has failed. Should not have occurred due to the MATCHED_ONLY selection mode.", Color::RED);
                                    }
                                }
                                catch (const KeypleReaderException &e1) {
                                    e1->fillInStackTrace();
                                }
                                catch (const std::runtime_error &e) {
                                    LOG->debug("Exception: " + e.what());
                                    appendColoredText(mText, "\nException: " + e.what(), Color::RED);
                                    e.fillInStackTrace();
                                }

                            }
                           );

                        }

                        void NFCTestFragment::onDestroy() {
                            Fragment::onDestroy();

                            LOG->debug("Remove task as an observer for ReaderEvents");
                            (std::static_pointer_cast<ObservableReader>(reader))->removeObserver(shared_from_this());

                            // destroy AndroidNFC fragment
                            std::shared_ptr<FragmentManager> fm = getFragmentManager();
                            std::shared_ptr<Fragment> f = fm->findFragmentByTag(TAG_NFC_ANDROID_FRAGMENT);
                            if (f != nullptr) {
                                fm->beginTransaction()->remove(f).commit();
                            }
                        }

                        void NFCTestFragment::initTextView() {
                            mText->setText(""); // reset
                            appendColoredText(mText, "Waiting for a smartcard...", Color::BLUE);
                            mText->append("\n ---- \n");
                        }

                        void NFCTestFragment::appendHexBuffer(std::shared_ptr<TextView> tv, std::vector<char> &ba) {
                            int start = tv->getText()->length();
                            tv->append(ByteArrayUtils::toHex(ba));
                            int end = tv->getText()->length();

                            std::shared_ptr<Spannable> spannableText = std::static_pointer_cast<Spannable>(tv->getText());

                            spannableText->setSpan(std::make_shared<TypefaceSpan>("monospace"), start, end, 0);
                            spannableText->setSpan(std::make_shared<RelativeSizeSpan>(0.70f), start, end, 0);
                        }

                        void NFCTestFragment::appendColoredText(std::shared_ptr<TextView> tv, const std::string &text, int color) {
                            int start = tv->getText()->length();
                            tv->append(text);
                            int end = tv->getText()->length();

                            std::shared_ptr<Spannable> spannableText = std::static_pointer_cast<Spannable>(tv->getText());
                            spannableText->setSpan(std::make_shared<ForegroundColorSpan>(color), start, end, 0);
                        }
                    }
                }
            }
        }
    }
}
