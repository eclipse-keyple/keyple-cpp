#include "CalypsoPo.h"
#include "PoSelectionRequest.h"
#include "SeResponse.h"
#include "GetDataFciRespPars.h"
#include "ByteArrayUtils.h"
#include "SeResponse.h"
#include "SelectionStatus.h"

/* Common */
#include "System.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                    using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                    using GetDataFciRespPars = org::eclipse::keyple::calypso::command::po::parser::GetDataFciRespPars;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                    CalypsoPo::CalypsoPo(std::shared_ptr<PoSelectionRequest> poSelectionRequest) : org::eclipse::keyple::transaction::MatchingSe(poSelectionRequest), poSelectionRequest(poSelectionRequest) {
                    }

                    void CalypsoPo::setSelectionResponse(std::shared_ptr<SeResponse> selectionResponse) {
                        MatchingSe::setSelectionResponse(selectionResponse);
                        /* Update the parser objects with the responses obtained */
                        poSelectionRequest->updateParsersWithResponses(selectionResponse);

                        /* The selectionSeResponse may not include a FCI field (e.g. old PO Calypso Rev 1) */
                        if (selectionResponse->getSelectionStatus()->getFci()->isSuccessful()) {
                            /* Parse PO FCI - to retrieve Calypso Revision, Serial Number, &amp; DF Name (AID) */
                            std::shared_ptr<GetDataFciRespPars> poFciRespPars = std::make_shared<GetDataFciRespPars>(selectionResponse->getSelectionStatus()->getFci());

                            /*
                             * Resolve the PO revision from the application type byte:
                             *
                             * <ul> <li>if
                             * <code>%1-------</code>&nbsp;&nbsp;&rarr;&nbsp;&nbsp;CLAP&nbsp;&nbsp;&rarr;&nbsp;&
                             * nbsp; REV3.1</li> <li>if
                             * <code>%00101---</code>&nbsp;&nbsp;&rarr;&nbsp;&nbsp;REV3.2</li> <li>if
                             * <code>%00100---</code>&nbsp;&nbsp;&rarr;&nbsp;&nbsp;REV3.1</li>
                             * <li>otherwise&nbsp;&nbsp;&rarr;&nbsp;&nbsp;REV2.4</li> </ul>
                             */
                            // TODO Improve this code by taking into account the startup information and the atr
                            char applicationTypeByte = poFciRespPars->getApplicationTypeByte();
                            if ((applicationTypeByte & (1 << 7)) != 0) {
                                /* CLAP */
                                this->revision = PoRevision::REV3_1_CLAP;
                            }
                            else if ((applicationTypeByte >> 3) == static_cast<char>(0x05)) {
                                this->revision = PoRevision::REV3_2;
                            }
                            else if ((applicationTypeByte >> 3) == static_cast<char>(0x04)) {
                                this->revision = PoRevision::REV3_1;
                            }
                            else {
                                this->revision = PoRevision::REV2_4;
                            }

                            this->dfName = poFciRespPars->getDfName();

                            this->applicationSerialNumber = poFciRespPars->getApplicationSerialNumber();

                            // TODO review this to take into consideration the type and subtype
                            if (this->revision == PoRevision::REV2_4) {
                                /* old cards have their modification counter in number of commands */
                                modificationCounterIsInBytes = false;
                                this->modificationsCounterMax = REV2_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION;
                            }
                            else {
                                this->modificationsCounterMax = poFciRespPars->getBufferSizeValue();
                            }
                        }
                        else {
                            /*
                             * FCI is not provided: we consider it is Calypso PO rev 1, it's serial number is
                             * provided in the ATR
                             */
                            poAtr = selectionResponse->getSelectionStatus()->getAtr()->getBytes();

                            /* basic check: we expect to be here following a selection based on the ATR */
                            if (poAtr.size() != PO_REV1_ATR_LENGTH) {
                                throw std::make_shared<IllegalStateException>("Unexpected ATR length: " + ByteArrayUtils::toHex(poAtr));
                            }

                            this->revision = PoRevision::REV1_0;
                            this->dfName.clear();
                            this->applicationSerialNumber = std::vector<char>(8);
                            /* old cards have their modification counter in number of commands */
                            this->modificationCounterIsInBytes = false;
                            this->modificationsCounterMax = REV1_PO_DEFAULT_WRITE_OPERATIONS_NUMBER_SUPPORTED_PER_SESSION;
                            /*
                             * the array is initialized with 0 (cf. default value for primitive types)
                             */
                            System::arraycopy(poAtr, 12, this->applicationSerialNumber, 4, 4);
                        }
                        if (logger->isTraceEnabled()) {
                            logger->trace("REVISION = %s, SERIALNUMBER = %s, DFNAME = %s", static_cast<int>(this->revision), ByteArrayUtils::toHex(this->applicationSerialNumber), ByteArrayUtils::toHex(this->dfName));
                        }
                    }

                    PoRevision CalypsoPo::getRevision() {
                        return this->revision;
                    }

                    std::vector<char> CalypsoPo::getDfName() {
                        return dfName;
                    }

                    std::vector<char> CalypsoPo::getApplicationSerialNumber() {
                        return applicationSerialNumber;
                    }

                    std::vector<char> CalypsoPo::getAtr() {
                        return poAtr;
                    }

                    bool CalypsoPo::isModificationsCounterInBytes() {
                        return modificationCounterIsInBytes;
                    }

                    int CalypsoPo::getModificationsCounter() {
                        return modificationsCounterMax;
                    }

                    PoClass CalypsoPo::getPoClass() {
                        /* Rev1 and Rev2 expects the legacy class byte while Rev3 expects the ISO class byte */
                        if (revision == PoRevision::REV1_0 || revision == PoRevision::REV2_4) {
                            if (logger->isTraceEnabled()) {
                                logger->trace("PO revision = %d, PO class = %s", static_cast<int>(revision), "PoClass::LEGACY");
                            }
                            return PoClass::LEGACY;
                        }
                        else {
                            if (logger->isTraceEnabled()) {
                                logger->trace("PO revision = %d, PO class = %s", static_cast<int>(revision), "PoClass::ISO");
                            }
                            return PoClass::ISO;
                        }
                    }

                    void CalypsoPo::reset() {
                        MatchingSe::reset();
                        applicationSerialNumber.clear();
                        poAtr.clear();
                        dfName.clear();
                    }
                }
            }
        }
    }
}
