#include "OpenSession32RespPars.h"
#include "ApduResponse.h"
#include "AbstractOpenSessionRespPars.h"

#include "Arrays.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::core::seproxy::message;
using namespace keyple::calypso::command::po::parser::security;

OpenSession32RespPars::OpenSession32RespPars(std::shared_ptr<ApduResponse> response)
: AbstractOpenSessionRespPars(response, PoRevision::REV3_2)
{
    std::vector<char> data = response->getDataOut();
    this->secureSession = toSecureSession(data);
}

std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> OpenSession32RespPars::toSecureSession(std::vector<char> &apduResponseData)
{
    return createSecureSession(apduResponseData);
}

std::shared_ptr<AbstractOpenSessionRespPars::SecureSession> OpenSession32RespPars::createSecureSession(std::vector<char> &apduResponse)
{

    char flag = apduResponse[8];
    // ratification: if the bit 0 of flag is set then the previous security has been ratified
    bool previousSessionRatified = (flag & (1 << 0)) == static_cast<char>(0x00);
    // secure security: if the bit 1 of flag is set then the secure security is authorized
    bool manageSecureSessionAuthorized = (flag & (1 << 1)) == static_cast<char>(0x02);

    char kif = apduResponse[9];
    int dataLength = apduResponse[11];
    std::vector<char> data = Arrays::copyOfRange(apduResponse, 12, 12 + dataLength);
    std::vector<char> challengeTransactionCounter = Arrays::copyOfRange(apduResponse, 0, 3);
    std::vector<char> challengeRandomNumber = Arrays::copyOfRange(apduResponse, 3, 8);

    return std::make_shared<SecureSession>(challengeTransactionCounter,
                                           challengeRandomNumber,
                                           previousSessionRatified, manageSecureSessionAuthorized,
                                           kif, std::make_shared<Byte>(apduResponse[10]),
                                           data, apduResponse);
}

}
}
}
}
}
}
