#include "PoResource.h"
#include "CalypsoPo.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy/SeReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using SeResource = org::eclipse::keyple::core::selection::SeResource;
                    using SeReader = org::eclipse::keyple::core::seproxy::SeReader;

                    PoResource::PoResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoPo> calypsoPo) : org::eclipse::keyple::core::selection::SeResource<CalypsoPo>(seReader, calypsoPo) {
                    }
                }
            }
        }
    }
}
