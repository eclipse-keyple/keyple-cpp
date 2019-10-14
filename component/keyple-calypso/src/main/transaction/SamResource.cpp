#include "SamResource.h"
#include "CalypsoSam.h"
#include "SeReader.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::selection;

SamResource::SamResource(std::shared_ptr<SeReader> seReader,
                         std::shared_ptr<CalypsoSam> calypsoSam)
: SeResource<CalypsoSam>(seReader, calypsoSam)
{
}

}
}
}
}
}
