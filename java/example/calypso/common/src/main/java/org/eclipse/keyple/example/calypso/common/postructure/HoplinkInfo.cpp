#include "HoplinkInfo.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace calypso {
                    namespace common {
                        namespace postructure {

const std::string HoplinkInfo::AID = "A000000291A000000191";
const std::string HoplinkInfo::SAM_C1_ATR_REGEX = "3B3F9600805A[0-9a-fA-F]{2}80C1[0-9a-fA-F]{14}829000";
const std::string HoplinkInfo::t2UsageRecord1_dataFill = std::string("0102030405060708090A0B0C0D0E0F10")
            + "1112131415161718191A1B1C1D1E1F20" + "2122232425262728292A2B2C2D2E2F30";
const std::string HoplinkInfo::EXTRAINFO_ReadRecord_T2UsageRec1 = std::string::format("T2 Usage (SFI=%02X, recnbr=%d))", SFI_T2Usage, RECORD_NUMBER_1);
const std::string HoplinkInfo::EXTRAINFO_ReadRecord_T2EnvironmentRec1 = std::string::format("T2 Environment (SFI=%02X, recnbr=%d))", SFI_T2Environment, RECORD_NUMBER_1);
                        }
                    }
                }
            }
        }
    }
}
