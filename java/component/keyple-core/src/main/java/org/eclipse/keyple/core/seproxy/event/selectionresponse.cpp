#include "SelectionResponse.h"
#include "SeResponseSet.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using SeResponseSet = org::eclipse::keyple::core::seproxy::message::SeResponseSet;

SelectionResponse::SelectionResponse(
                         std::shared_ptr<SeResponseSet> selectionSeResponseSet)
: selectionSeResponseSet(selectionSeResponseSet)
{
}

std::shared_ptr<SeResponseSet> SelectionResponse::getSelectionSeResponseSet()
{
    return selectionSeResponseSet;
}

}
}
}
}
}
}
