#include "CalypsoPo.h"
#include "PoResource.h"
#include "SeReader.h"
#include "SeResource.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using namespace org::eclipse::keyple::core::selection;
using namespace org::eclipse::keyple::core::seproxy;

PoResource::PoResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoPo> calypsoPo)
: SeResource<CalypsoPo>(seReader, calypsoPo)
{
}

}
}
}
}
}
