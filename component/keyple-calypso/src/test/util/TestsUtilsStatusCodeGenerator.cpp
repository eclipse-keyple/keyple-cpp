#include "TestsUtilsStatusCodeGenerator.h"

namespace keyple {
namespace calypso {
namespace util {

std::vector<char> TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()
{
    return std::vector<char>{static_cast<char>(0x90), 0x00};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateCommandForbiddenOnBinaryFilesStatusCode()
{
    return std::vector<char>{static_cast<char>(0x69), static_cast<char>(0x81)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateFileNotFoundStatusCode()
{
    return std::vector<char>{static_cast<char>(0x69), static_cast<char>(0x82)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateRecordNotFoundStatusCode()
{
    return std::vector<char>{static_cast<char>(0x6A), static_cast<char>(0x83)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateP2ValueNotSupportedStatusCode()
{
    return std::vector<char>{static_cast<char>(0x6B), 0x00};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateLeValueIncorrectStatusCode()
{
    return std::vector<char>{static_cast<char>(0x6C), static_cast<char>(0xFF)};
}

std::vector<char>
TestsUtilsStatusCodeGenerator::generateAccessForbiddenStatusCode()
{
    return std::vector<char>{static_cast<char>(0x69), static_cast<char>(0x85)};
}
}
}
}
