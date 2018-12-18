#include "TestsUtilsResponseTabByteGenerator.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "TestsUtilsStatusCodeGenerator.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace util {
                    using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    std::vector<char> TestsUtilsResponseTabByteGenerator::append(std::vector<char> &arrays) {
                        std::shared_ptr<ByteArrayOutputStream> * const out = std::make_shared<ByteArrayOutputStream>();
                        if (arrays != nullptr) {
                            for (char[] array_Renamed : arrays) {
                                if (array_Renamed.size() > 0) {
                                    out->write(array_Renamed, 0, array_Renamed.size());
                                }
                            }
                        }
                        return out->toByteArray();
                    }

                    std::vector<char> TestsUtilsResponseTabByteGenerator::generate4MultiRecordsTabByte() {
                        return std::vector<char> {0x01, 0x01, 0x01, 0x01, 0x30, 0x01, 0x01, 0x30, 0x01, 0x01, 0x30, 0x01, 0x01, 0x30};
                    }

                    std::vector<char> TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev2_4() {
                        return ByteArrayUtils::fromHex("CC 11223344 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCC 9000");
                    }

                    std::vector<char> TestsUtilsResponseTabByteGenerator::generateDataOpenTabByte() {
                        return std::vector<char> {0x7E, static_cast<char>(0x8F), 0x05, 0x75, 0x01A, static_cast<char>(0xFF), 0x01, 0x01, 0x00, 0x30};
                    }

                    std::vector<char> TestsUtilsResponseTabByteGenerator::generateFciTabByte() {
                        return std::vector<char> {static_cast<char>(0x6F), 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB7), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01};
                    }

                    std::vector<char> TestsUtilsResponseTabByteGenerator::generateTerminalSessionSignatureTabByte() {
                        return std::vector<char> {0x7E, static_cast<char>(0x8F), 0x05, 0x75, 0x01A, static_cast<char>(0xFF), 0x01, 0x01};
                    }

                    std::vector<char> TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev3_1() {
                        return std::vector<char> {0x6F, 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x1E, 0x2E, 0x49, 0x43, 0x41, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB9), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01, static_cast<char>(0x90), 0x00};
                    }

                    std::vector<char> TestsUtilsResponseTabByteGenerator::generateResponseOkTabByteRev3_2() {
                        return std::vector<char> {0x6F, 0x22, static_cast<char>(0x84), 0x08, 0x33, 0x4D, 0x54, 0x52, 0x2E, 0x49, 0x1B, 0x1A, static_cast<char>(0xA5), 0x16, static_cast<char>(0xBF), 0x0C, 0x13, static_cast<char>(0xC7), 0x08, 0x00, 0x00, 0x00, 0x00, 0x27, 0x4A, static_cast<char>(0x9A), static_cast<char>(0xB9), 0x53, 0x07, 0x0A, 0x3C, 0x11, 0x32, 0x14, 0x10, 0x01, static_cast<char>(0x90), 0x00};
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev2_4() {
                        return std::make_shared<ApduResponse>(append({generateResponseOkTabByteRev2_4(), TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_1() {
                        return std::make_shared<ApduResponse>(append({generateResponseOkTabByteRev3_1(), TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_2() {
                        return std::make_shared<ApduResponse>(append({generateResponseOkTabByteRev3_2(), TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseOpenSessionCmd() {
                        return std::make_shared<ApduResponse>(append({generateDataOpenTabByte(), TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseOpenSessionCmdError() {
                        return std::make_shared<ApduResponse>(append({generateDataOpenTabByte(), TestsUtilsStatusCodeGenerator::generateAccessForbiddenStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseTerminalSessionSignatureCmd() {
                        return std::make_shared<ApduResponse>(append({generateTerminalSessionSignatureTabByte(), TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseTerminalSessionSignatureCmdError() {
                        return std::make_shared<ApduResponse>(append({generateTerminalSessionSignatureTabByte(), TestsUtilsStatusCodeGenerator::generateCommandForbiddenOnBinaryFilesStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseFciCmd() {
                        return std::make_shared<ApduResponse>(append({generateFciTabByte(), TestsUtilsStatusCodeGenerator::generateSuccessfulStatusCode()}), nullptr);
                    }

                    std::shared_ptr<ApduResponse> TestsUtilsResponseTabByteGenerator::generateApduResponseFciCmdError() {
                        return std::make_shared<ApduResponse>(append({generateFciTabByte(), TestsUtilsStatusCodeGenerator::generateFileNotFoundStatusCode()}), nullptr);
                    }
                }
            }
        }
    }
}
