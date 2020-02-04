#include "SelectDiversifierRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;

SelectDiversifierRespPars::SelectDiversifierRespPars(
  std::shared_ptr<ApduResponse> response)
: AbstractSamResponseParser(response)
{
}

}
}
}
}
}
}
