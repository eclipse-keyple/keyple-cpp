#include "SamResource.h"
#include "CalypsoSam.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/core/seproxy/SeReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using SeResource = org::eclipse::keyple::core::selection::SeResource;
                    using SeReader = org::eclipse::keyple::core::seproxy::SeReader;

                    SamResource::SamResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoSam> calypsoSam) : org::eclipse::keyple::core::selection::SeResource<CalypsoSam>(seReader, calypsoSam) {
                    }
                }
            }
        }
    }
}
