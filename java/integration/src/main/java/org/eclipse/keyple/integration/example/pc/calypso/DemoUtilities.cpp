#include "DemoUtilities.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace example {
                    namespace pc {
                        namespace calypso {
                            using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                            using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
std::string DemoUtilities::PO_READER_NAME_REGEX = ".*(ASK|ACS).*";
std::string DemoUtilities::SAM_READER_NAME_REGEX = ".*(Cherry TC|SCM Microsystems|Identive|HID|Generic).*";

                            std::shared_ptr<SeReader> DemoUtilities::getReader(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern) throw(KeypleReaderException) {
                                std::shared_ptr<Pattern> p = Pattern::compile(pattern);
                                for (auto plugin : seProxyService->getPlugins()) {
                                    for (auto reader : plugin->getReaders()) {
                                        if (p->matcher(reader->getName()).matches()) {
                                            return reader;
                                        }
                                    }
                                }
                                return nullptr;
                            }
                        }
                    }
                }
            }
        }
    }
}
