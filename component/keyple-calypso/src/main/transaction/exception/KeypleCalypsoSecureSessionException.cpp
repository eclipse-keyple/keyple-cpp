/* Core */
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "KeypleReaderException.h"

/* Calypso */
#include "KeypleCalypsoSecureSessionException.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {
namespace exception {

using KeypleReaderException = org::eclipse::keyple::core::seproxy::exception::KeypleReaderException;
using ApduRequest           = org::eclipse::keyple::core::seproxy::message::ApduRequest;
using ApduResponse          = org::eclipse::keyple::core::seproxy::message::ApduResponse;

KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(const std::string &message, Type type, std::vector<std::shared_ptr<ApduRequest>> &requests, std::vector<std::shared_ptr<ApduResponse>> &responses)
: KeypleReaderException(message), type(type), requests(requests), responses(responses) {
}

KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(const std::string &message, std::shared_ptr<ApduRequest> req, std::shared_ptr<ApduResponse> resp)
: KeypleReaderException(message)
{
    requests.push_back(req);
    responses.push_back(resp);
    type = Type::NO_TYPE; 
}

KeypleCalypsoSecureSessionException::KeypleCalypsoSecureSessionException(
                                    const std::string &message, std::shared_ptr<ApduResponse> resp)
: KeypleCalypsoSecureSessionException(message, nullptr, resp)
{
}

KeypleCalypsoSecureSessionException::Type KeypleCalypsoSecureSessionException::getType()
{
    return type;
}

std::vector<std::shared_ptr<ApduRequest>> KeypleCalypsoSecureSessionException::getRequests()
{
    return requests;
}

std::vector<std::shared_ptr<ApduResponse>> KeypleCalypsoSecureSessionException::getResponses()
{
    return responses;
}

}
}
}
}
}
}
