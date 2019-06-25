#include "DefaultSelectionsRequest.h"
#include "ObservableReader.h"
#include "SeRequestSet.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using AbstractDefaultSelectionsRequest = org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsRequest;
using ObservableReader = org::eclipse::keyple::core::seproxy::event::ObservableReader;

DefaultSelectionsRequest::DefaultSelectionsRequest(std::shared_ptr<SeRequestSet> selectionSeRequestSet) : org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsRequest(selectionSeRequestSet) {
}

std::shared_ptr<SeRequestSet> DefaultSelectionsRequest::getSelectionSeRequestSet() {
    return selectionSeRequestSet;
}

}
}
}
}
}
}
