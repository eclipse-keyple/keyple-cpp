#include "AbstractDefaultSelectionsRequest.h"
#include "SeRequestSet.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using SeRequestSet = org::eclipse::keyple::core::seproxy::message::SeRequestSet;

AbstractDefaultSelectionsRequest::AbstractDefaultSelectionsRequest(std::shared_ptr<SeRequestSet> selectionSeRequestSet) : selectionSeRequestSet(selectionSeRequestSet) {
}

}
}
}
}
}
}
