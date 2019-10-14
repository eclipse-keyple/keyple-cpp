#include "NativeReaderService.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                    }
                }
            }
        }
    }
}
