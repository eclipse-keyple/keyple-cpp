#include "DefaultSelectionsResponse.h"
#include "ObservableReader.h"
#include "SeResponseSet.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using AbstractDefaultSelectionsResponse = org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsResponse;
using ObservableReader = org::eclipse::keyple::core::seproxy::event::ObservableReader;

DefaultSelectionsResponse::DefaultSelectionsResponse(std::shared_ptr<SeResponseSet> selectionSeResponseSet) : org::eclipse::keyple::core::seproxy::event::AbstractDefaultSelectionsResponse(selectionSeResponseSet) {
}

std::shared_ptr<SeResponseSet> DefaultSelectionsResponse::getSelectionSeResponseSet() {
    return selectionSeResponseSet;
}

}
}
}
}
}
}
