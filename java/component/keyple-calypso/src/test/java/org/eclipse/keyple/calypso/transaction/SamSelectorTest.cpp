#include "SamSelectorTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/SamRevision.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/transaction/sam/SamSelector.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
//                    import static org.junit.Assert.*;
                    using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;
                    using SamSelector = org::eclipse::keyple::calypso::transaction::sam::SamSelector;
                    using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_AidSelectorNull()
                    void SamSelectorTest::test_AidSelectorNull() {
                        std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::S1D, nullptr, "S1D");
                        assertNull(samSelector->getAidSelector());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_S1D()
                    void SamSelectorTest::test_SamRevision_S1D() {
                        std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::S1D, nullptr, "S1D");
                        assertEquals("3B(.{6}|.{10})805A..80D?20.{4}.{8}829000", samSelector->getAtrFilter()->getAtrRegex());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_S1E()
                    void SamSelectorTest::test_SamRevision_S1E() {
                        std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::S1E, nullptr, "S1E");
                        assertEquals("3B(.{6}|.{10})805A..80E120.{4}.{8}829000", samSelector->getAtrFilter()->getAtrRegex());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_C1()
                    void SamSelectorTest::test_SamRevision_C1() {
                        std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::C1, nullptr, "C1");
                        assertEquals("3B(.{6}|.{10})805A..80C120.{4}.{8}829000", samSelector->getAtrFilter()->getAtrRegex());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_ANY()
                    void SamSelectorTest::test_SamRevision_ANY() {
                        std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::AUTO, nullptr, "Any");
                        assertEquals(".*", samSelector->getAtrFilter()->getAtrRegex());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamSerialNumber()
                    void SamSelectorTest::test_SamSerialNumber() {
                        std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::C1, "11223344", "Any");
                        assertEquals("3B(.{6}|.{10})805A..80C120.{4}11223344829000", samSelector->getAtrFilter()->getAtrRegex());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_ExtraInfo()
                    void SamSelectorTest::test_ExtraInfo() {
                        std::shared_ptr<SamSelector> samSelector = std::make_shared<SamSelector>(SamRevision::AUTO, nullptr, "Any");
                        assertEquals("Any", samSelector->getExtraInfo());
                    }
                }
            }
        }
    }
}
