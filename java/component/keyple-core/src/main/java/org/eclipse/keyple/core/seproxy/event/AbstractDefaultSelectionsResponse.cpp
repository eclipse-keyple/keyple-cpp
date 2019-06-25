#include "AbstractDefaultSelectionsResponse.h"
#include "SeResponseSet.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using SeResponseSet = org::eclipse::keyple::core::seproxy::message::SeResponseSet;

AbstractDefaultSelectionsResponse::AbstractDefaultSelectionsResponse(std::shared_ptr<SeResponseSet> selectionSeResponseSet) : selectionSeResponseSet(selectionSeResponseSet) {
}

}
}
}
}
}
}
