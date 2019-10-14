#include "gmock/gmock.h"

/* Core */
#include "NoStackTraceThrowable.h"

/* Smartcard I/O */
#include "SmartCardIOReaderTest.h"

using testing::Return;
using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;

TEST_F(SmartCardIOReaderTest, testSmartCardIOReader)
{
    ASSERT_NE(reader, nullptr);
}

/* TODO redesign
TEST_F(SmartCardIOReaderTest, testGettersSetters) //throw(std::invalid_argument, KeypleBaseException)
{
    // this.reader = new PcscReader(terminal, readerName);
    reader->setParameter("TOTO", "TOTO");
    ASSERT_EQ(reader->getParameters().size(), 1);

    std::unordered_map<std::string, std::string> test;
    test.emplace("TITI", "TITI");
    test.emplace("TATA", "TATA");
    reader->setParameters(test);
    ASSERT_EQ(reader->getParameters().size(), 3);

    ASSERT_TRUE(readerName == reader->getName());
}

void SmartCardIOReaderTest::testIsSEPresent()
throw(CardException, NoStackTraceThrowable)
{
    when(terminal->isCardPresent()).thenReturn(true);
    assertTrue(reader->isSePresent());
    when(terminal->isCardPresent()).thenReturn(false);
    assertFalse(reader->isSePresent());
}

void SmartCardIOReaderTest::testIsSEPresentWithException()
throw(CardException, NoStackTraceThrowable)
{

    when(terminal->waitForCardAbsent(0)).thenReturn(false);
//JAVA TO C++ CONVERTER TODO TASK: This exception's constructor requires an argument:
//ORIGINAL LINE: doThrow(new CardException("erreur", new Exception())).when(terminal).isCardPresent();
    doThrow(std::make_shared<CardException>("erreur", std::runtime_error())).when(terminal).isCardPresent();
    reader->isSePresent();


}

TODO redisign
void SmartCardIOReaderTest::testTransmitCardNotPresent() throw(CardException, KeypleReaderException, KeypleReaderException) {

    when(terminal->isCardPresent()).thenReturn(false);
    std::shared_ptr<ApduRequest> apduRequestMF = std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("94A40000023F02"), false);

    // code de la requete
    std::vector<char> aidToSelect(0);

    std::vector<std::shared_ptr<ApduRequest>> apduRequests;
    apduRequests.push_back(apduRequestMF);
    std::shared_ptr<SeRequestSet> seApplicationRequest = std::make_shared<SeRequestSet>(std::make_shared<SeRequest>(apduRequests, ChannelState::KEEP_OPEN));

    std::shared_ptr<SeResponseSet> reponseActuelle = reader->transmitSet(seApplicationRequest);

    assertNull(reponseActuelle->getSingleResponse()->getSelectionStatus()->getFci());
    assertEquals(reponseActuelle->getSingleResponse()->getApduResponses().size(), 0);
    assertFalse(reponseActuelle->getSingleResponse()->wasChannelPreviouslyOpen());
}

void SmartCardIOReaderTest::testTransmitToCardWithoutAidToSelect() throw(CardException, KeypleReaderException, KeypleReaderException) {

    atr = std::make_shared<ATR>(ByteArrayUtils::fromHex("85170001"));
    when(terminal->isCardPresent()).thenReturn(true);
    when(channel->transmit(any(CommandAPDU::typeid))).thenReturn(res);
    when(card->getATR()).thenReturn(atr);
    // this.reader = new PcscReader(terminal, readerName);
    std::vector<char> returnOK = {static_cast<char>(0x90), static_cast<char>(0x00)};
    std::shared_ptr<ApduResponse> responseMockMF = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("0x851700010000001212000001030101007E7E7E0000000000009000"), nullptr);
    std::shared_ptr<ApduRequest> apduRequestMF = std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("94A40000023F02"), false);

    // code de la requete
    std::vector<char> aidToSelect;

    std::vector<std::shared_ptr<ApduRequest>> apduRequests;
    apduRequests.push_back(apduRequestMF);
    std::shared_ptr<SeRequestSet> seApplicationRequest = std::make_shared<SeRequestSet>(std::make_shared<SeRequest>(apduRequests, ChannelState::KEEP_OPEN));

    std::shared_ptr<PcscReader> spiedReader = spy(this->reader);
    std::shared_ptr<SeResponseSet> reponseActuelle = spiedReader->transmitSet(seApplicationRequest);

    assertEquals(reponseActuelle->getSingleResponse()->getApduResponses().size(), seApplicationRequest->getSingleRequest()->getApduRequests().size());
    // assertNotNull(Whitebox.getInternalState(spiedReader, "card"));
    // assertNotNull(Whitebox.getInternalState(spiedReader, "channel"));
    assertNotNull(reponseActuelle->getSingleResponse()->getSelectionStatus()->getFci());
}

void SmartCardIOReaderTest::testTransmitToCardWithAidToSelect() throw(CardException, KeypleReaderException, KeypleReaderException) {


    when(terminal->isCardPresent()).thenReturn(true);
    when(channel->transmit(any(CommandAPDU::typeid))).thenReturn(res);
    atr = std::make_shared<ATR>(ByteArrayUtils::fromHex("85170001"));
    when(card->getATR()).thenReturn(atr);
    std::vector<char> returnOK = {static_cast<char>(0x90), static_cast<char>(0x00)};
    std::shared_ptr<ApduResponse> responseMockMF = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("0x851700010000001212000001030101007E7E7E0000000000009000"), nullptr);
    std::shared_ptr<ApduRequest> apduRequestMF = std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("94A40000023F02"), false);

    // code de la requete
    std::vector<char> aidToSelect = ByteArrayUtils::fromHex("94CA004F00");

    std::vector<std::shared_ptr<ApduRequest>> apduRequests;
    apduRequests.push_back(apduRequestMF);
    std::shared_ptr<SeRequestSet> seApplicationRequest = std::make_shared<SeRequestSet>(std::make_shared<SeRequest>(apduRequests, ChannelState::KEEP_OPEN));

    std::shared_ptr<PcscReader> spiedReader = spy(this->reader);

    std::shared_ptr<SeResponseSet> reponseActuelle = spiedReader->transmitSet(seApplicationRequest);
    assertNotNull(reponseActuelle->getSingleResponse()->getSelectionStatus()->getFci());
    assertEquals(reponseActuelle->getSingleResponse()->getApduResponses().size(), seApplicationRequest->getSingleRequest()->getApduRequests().size());
}

void SmartCardIOReaderTest::testTransmitToCardAndDisconnect() throw(CardException, KeypleReaderException, KeypleReaderException) {


    when(terminal->isCardPresent()).thenReturn(true);
    when(channel->transmit(any(CommandAPDU::typeid))).thenReturn(res);
    std::vector<char> returnOK = {static_cast<char>(0x90), static_cast<char>(0x00)};
    std::shared_ptr<ApduResponse> responseMockMF = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("0x851700010000001212000001030101007E7E7E0000000000009000"), nullptr);
    std::shared_ptr<ApduRequest> apduRequestMF = std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("94A40000023F02"), false);
    // code de la requete
    std::vector<char> aidToSelect = ByteArrayUtils::fromHex("94CA004F00");

    std::vector<std::shared_ptr<ApduRequest>> apduRequests;
    apduRequests.push_back(apduRequestMF);
    std::shared_ptr<SeRequestSet> seApplicationRequest = std::make_shared<SeRequestSet>(std::make_shared<SeRequest>(apduRequests, ChannelState::KEEP_OPEN));

    std::shared_ptr<PcscReader> spiedReader = spy(this->reader);

    std::shared_ptr<SeResponseSet> reponseActuelle = spiedReader->transmitSet(seApplicationRequest);
    assertNotNull(reponseActuelle->getSingleResponse()->getSelectionStatus()->getFci());
    assertEquals(reponseActuelle->getSingleResponse()->getApduResponses().size(), seApplicationRequest->getSingleRequest()->getApduRequests().size());
    // assertNull(Whitebox.getInternalState(spiedReader, "card"));
    // assertNull(Whitebox.getInternalState(spiedReader, "channel"));
}
*/
