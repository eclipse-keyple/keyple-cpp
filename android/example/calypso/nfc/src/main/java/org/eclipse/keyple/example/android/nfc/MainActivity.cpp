#include "MainActivity.h"
#include "NFCTestFragment.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace android {
                    namespace nfc {
                        using AndroidNfcPlugin = org::eclipse::keyple::plugin::android::nfc::AndroidNfcPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using android::app::Fragment;
                        using android::app::FragmentManager;
                        using android::os::Bundle;
                        using android::support::annotation::NonNull;
                        using android::support::design::widget::NavigationView;
                        using android::support::v4::view::GravityCompat;
                        using android::support::v4::widget::DrawerLayout;
                        using android::support::v7::app::ActionBar;
                        using android::support::v7::app::AppCompatActivity;
                        using android::support::v7::widget::Toolbar;
                        using android::view::MenuItem;
                        using android::view::WindowManager;
const std::shared_ptr<org::slf4j::Logger> MainActivity::LOG = org::slf4j::LoggerFactory::getLogger(MainActivity::typeid);
const std::string MainActivity::TAG_NFC_TEST_VIEW = "tagnfctestfragment";

                        void MainActivity::onCreate(std::shared_ptr<Bundle> savedInstanceState) {
                            AppCompatActivity::onCreate(savedInstanceState);

                            getWindow().addFlags(WindowManager::LayoutParams::FLAG_KEEP_SCREEN_ON);

                            /*
                             * Define UI elements
                             */
                            LOG->debug("onCreate");
                            setContentView(org::eclipse::keyple::example::android::nfc::R::layout::activity_main);

                            setupNavDrawer();
                            /*
                             * end of Define UI elements
                             */

                            activateNFCTestView();


                        }

                        void MainActivity::activateNFCTestView() {
                            // init NFC Test Fragment
                            LOG->debug("Insert NFC Test View Fragment");

                            std::shared_ptr<FragmentManager> fm = getFragmentManager();
                            std::shared_ptr<Fragment> nfcTestFragment = fm->findFragmentByTag(TAG_NFC_TEST_VIEW);

                            if (nfcTestFragment == nullptr) {
                                LOG->debug("Create a new NFCTestFragment");
                                fm->beginTransaction()->replace(org::eclipse::keyple::example::android::nfc::R::id::fragment_container, NFCTestFragment::newInstance(), TAG_NFC_TEST_VIEW).addToBackStack(nullptr).commit();
                            }
                            else {
                                LOG->debug("NFCTestFragment is already created");
                            }
                        }

                        void MainActivity::setupNavDrawer() {
                            mDrawerLayout = findViewById(org::eclipse::keyple::example::android::nfc::R::id::drawer_layout);

                            // Drawer configuration
                            std::shared_ptr<NavigationView> navigationView = findViewById(org::eclipse::keyple::example::android::nfc::R::id::nav_view);
                            navigationView->setNavigationItemSelectedListener(std::make_shared<OnNavigationItemSelectedListenerAnonymousInnerClass>(shared_from_this()));

                            // Toolbar
                            std::shared_ptr<Toolbar> toolbar = findViewById(org::eclipse::keyple::example::android::nfc::R::id::toolbar);
                            setSupportActionBar(toolbar);

                            // Hamburger icon
                            std::shared_ptr<ActionBar> actionbar = getSupportActionBar();
                            assert(actionbar != nullptr);
                            actionbar->setDisplayHomeAsUpEnabled(true);
                            actionbar->setHomeAsUpIndicator(org::eclipse::keyple::example::android::nfc::R::drawable::ic_menu_black_24dp);

                        }

                        MainActivity::OnNavigationItemSelectedListenerAnonymousInnerClass::OnNavigationItemSelectedListenerAnonymousInnerClass(std::shared_ptr<MainActivity> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Override public boolean onNavigationItemSelected(@NonNull android.view.MenuItem menuItem)
                        bool MainActivity::OnNavigationItemSelectedListenerAnonymousInnerClass::onNavigationItemSelected(std::shared_ptr<MenuItem> menuItem) {
                            // set item as selected to persist highlight
                            menuItem->setChecked(true);
                            // close drawer when item is tapped
                            outerInstance->mDrawerLayout->closeDrawers();

                            // Add code here to update the UI based on the item selected
                            // For example, swap UI fragments here
                            LOG->debug("Item selected from drawer: " + menuItem->getTitle());

                            switch (menuItem->getItemId()) {
                                case org::eclipse::keyple::example::android::nfc::R::id::nav_nfc:
                                    outerInstance->activateNFCTestView();
                                    break;

                                default:
                                    throw std::invalid_argument("Invalid menuItem");

                            }
                            return true;
                        }

                        bool MainActivity::onOptionsItemSelected(std::shared_ptr<MenuItem> item) {
                            switch (item->getItemId()) {
                                case android::R::id::home:
                                    mDrawerLayout->openDrawer(GravityCompat::START);
                                    return true;
                            }
                            return AppCompatActivity::onOptionsItemSelected(item);
                        }
                    }
                }
            }
        }
    }
}
