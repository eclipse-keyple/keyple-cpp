#include "MainActivity.h"
#include "OMAPITestFragment.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace android {
                    namespace omapi {
                        using android::os::Bundle;
                        using android::support::design::widget::NavigationView;
                        using android::support::v4::view::GravityCompat;
                        using android::support::v4::widget::DrawerLayout;
                        using android::support::v7::app::ActionBar;
                        using android::support::v7::app::AppCompatActivity;
                        using android::support::v7::widget::Toolbar;
                        using android::util::Log;
                        using android::view::MenuItem;
const std::string MainActivity::TAG = MainActivity::typeid->getSimpleName();
const std::string MainActivity::TAG_OMAPI_TEST_VIEW = "tagomapitestfragment";

                        void MainActivity::onCreate(std::shared_ptr<Bundle> savedInstanceState) {
                            AppCompatActivity::onCreate(savedInstanceState);

                            /**
                             * Define UI elements
                             */
                            Log::d(TAG, "onCreate");
                            setContentView(R::layout::activity_main);

                            setupNavDrawer();
                            /**
                             * end of Define UI elements
                             */
                            activateOMAPITestView();

                        }

                        void MainActivity::activateOMAPITestView() {
                            // init OMAPI Test Fragment
                            Log::d(TAG, "Insert OMAPI Test View  Fragment");
                            std::shared_ptr<OMAPITestFragment> omapiTestFragment = OMAPITestFragment::newInstance();
                            getFragmentManager().beginTransaction()->replace(R::id::fragment_container, omapiTestFragment, TAG_OMAPI_TEST_VIEW).addToBackStack(nullptr).commit();
                        }

                        void MainActivity::setupNavDrawer() {
                            mDrawerLayout = findViewById(R::id::drawer_layout);

                            std::shared_ptr<NavigationView> navigationView = findViewById(R::id::nav_view);
                            navigationView->setNavigationItemSelectedListener(std::make_shared<OnNavigationItemSelectedListenerAnonymousInnerClass>(shared_from_this()));

                            std::shared_ptr<Toolbar> toolbar = findViewById(R::id::toolbar);
                            setSupportActionBar(toolbar);

                            std::shared_ptr<ActionBar> actionbar = getSupportActionBar();
                            actionbar->setDisplayHomeAsUpEnabled(true);
                            actionbar->setHomeAsUpIndicator(R::drawable::ic_menu_black_24dp);



                        }

                        MainActivity::OnNavigationItemSelectedListenerAnonymousInnerClass::OnNavigationItemSelectedListenerAnonymousInnerClass(std::shared_ptr<MainActivity> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        bool MainActivity::OnNavigationItemSelectedListenerAnonymousInnerClass::onNavigationItemSelected(std::shared_ptr<MenuItem> menuItem) {
                            // set item as selected to persist highlight
                            menuItem->setChecked(true);
                            // close drawer when item is tapped
                            outerInstance->mDrawerLayout->closeDrawers();

                            // Add code here to update the UI based on the item selected
                            // For example, swap UI fragments here
                            Log::d(TAG, "Item selected from drawer: " + menuItem->getTitle());

                            switch (menuItem->getItemId()) {
                                case R::id::nav_omapi:
                                    outerInstance->activateOMAPITestView();
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
