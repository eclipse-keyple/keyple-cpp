#include "SeProxyServiceTest.h"
#include "../../../../../../main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../main/java/org/eclipse/keyple/seproxy/exception/KeyplePluginNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
//                import static org.junit.Assert.*;
//                import static org.mockito.Mockito.when;
                using KeyplePluginNotFoundException = org::eclipse::keyple::seproxy::exception::KeyplePluginNotFoundException;
                using org::junit::Before;
                using org::junit::Test;
                using org::junit::runner::RunWith;
                using org::mockito::Mock;
                using org::mockito::junit::MockitoJUnitRunner;
                using org::slf4j::Logger;
                using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> SeProxyServiceTest::logger = org::slf4j::LoggerFactory::getLogger(SeProxyServiceTest::typeid);
std::string SeProxyServiceTest::PLUGIN_NAME = "plugin1";

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setupBeforeEach()
                void SeProxyServiceTest::setupBeforeEach() {

                    // init class to test
                    proxyService = SeProxyService::getInstance();
                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetInstance()
                void SeProxyServiceTest::testGetInstance() {
                    // test
                    assertNotNull(proxyService);
                    // assertNull(proxyService);

                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetVersion()
                void SeProxyServiceTest::testGetVersion() {
                    // test that version follows semver guidelines
                    std::string regex = "^([0-9]+)\\.([0-9]+)\\.([0-9]+)(?:-([0-9A-Za-z-]+(?:\\.[0-9A-Za-z-]+)*))?(?:\\+[0-9A-Za-z-]+)?$";
                    std::string version = SeProxyService::getInstance()->getVersion();
                    logger->info("Version of SeProxyService {}", version);
                    std::cout << "Version of SeProxyService " << version << std::endl;
                    assertTrue(std::regex_match(version, std::regex(regex)));
                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetSetPlugins()
                void SeProxyServiceTest::testGetSetPlugins() {
                    // init
                    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>> plugins = getPluginList();

                    // test
                    proxyService->setPlugins(plugins);
                    assertArrayEquals(plugins->toArray(), proxyService->getPlugins()->toArray());
                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetPlugin() throws org.eclipse.keyple.seproxy.exception.KeyplePluginNotFoundException
                void SeProxyServiceTest::testGetPlugin() throw(KeyplePluginNotFoundException) {
                    // init

                    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>> plugins = getPluginList();

                    proxyService->setPlugins(plugins);

                    // test
                    assertEquals(plugin1, proxyService->getPlugin(PLUGIN_NAME));
                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeyplePluginNotFoundException.class) public void testGetPluginFail() throws Exception
                void SeProxyServiceTest::testGetPluginFail() throw(std::runtime_error) {

                    // init
                    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>> plugins = getPluginList();
                    proxyService->setPlugins(plugins);

                    // test
                    proxyService->getPlugin("unknown"); // Throw exception


                }

                std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>> SeProxyServiceTest::getPluginList() {

                    // ReaderPlugin plugin2 = Mockito.mock(ReaderPlugin.class);
                    // when(plugin2.getName()).thenReturn(PLUGIN_NAME_2);

                    when(plugin1->getName()).thenReturn(PLUGIN_NAME);
                    std::shared_ptr<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>> plugins = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();


                    plugins->add(plugin1);
                    // plugins.add(plugin2);

                    assertEquals(1, plugins->size()); // impossible to add 2 ReaderPlugin mocks

                    return plugins;
                }
            }
        }
    }
}
