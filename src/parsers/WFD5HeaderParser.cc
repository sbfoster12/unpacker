#include "WFD5HeaderParser.hh"

using namespace parsers;

WFD5HeaderParser::WFD5HeaderParser() : Parser() {};

WFD5HeaderParser::~WFD5HeaderParser() {};

//Method to create shared ptr
std::unique_ptr<dataProducts::WFD5Header> 
WFD5HeaderParser::NewDataProduct(unsigned int crateNum) {
    return std::make_unique<dataProducts::WFD5Header>(
                crateNum
                ,AMCNo()
                ,LV1ID()
                ,ClockCounter()
                ,TriggerType()
                ,BoardType()
                ,BoardID()
                ,XADCAlarm()
                ,EmptyEvent()
                ,MajorRevision()
                ,MinorRevision()
                ,PatchRevision()
            );
}

//Get methods
uint32_t        WFD5HeaderParser::AMCNo() const { return (GetWord(AMCNo_WORD) >> AMCNo_BIT) & AMCNo_MASK; }
uint32_t        WFD5HeaderParser::LV1ID() const { return (GetWord(LV1ID_WORD) >> LV1ID_BIT) & LV1ID_MASK; }
uint32_t        WFD5HeaderParser::BcN() const { return (GetWord(BcN_WORD) >> BcN_BIT) & BcN_MASK; }
uint32_t        WFD5HeaderParser::OrN() const { return (GetWord(OrN_WORD) >> OrN_BIT) & OrN_MASK; }
uint64_t        WFD5HeaderParser::ClockCounter() const { return ((uint64_t)BcN() << 32) | ((uint64_t)OrN()); }
uint32_t        WFD5HeaderParser::DataLength() const {return (GetWord(DataLength_WORD) >> DataLength_BIT) & DataLength_MASK;}
uint32_t        WFD5HeaderParser::TriggerType() const { return (GetWord(TriggerType_WORD) >> TriggerType_BIT) & TriggerType_MASK; }
uint32_t        WFD5HeaderParser::Endianness() const { return (GetWord(Endianness_WORD) >> Endianness_BIT) & Endianness_MASK; }
uint32_t        WFD5HeaderParser::BoardID() const { return (GetWord(BoardID_WORD) >> BoardID_BIT) & BoardID_MASK; }
uint32_t        WFD5HeaderParser::BoardType() const {return (GetWord(BoardType_WORD) >> BoardType_BIT) & BoardType_MASK; }
uint32_t        WFD5HeaderParser::ASyncMode() const {return (GetWord(ASyncMode_WORD) >> ASyncMode_BIT) & ASyncMode_MASK; }
uint32_t        WFD5HeaderParser::CRC() const { return (GetWord(GetSize() - 1) >> CRC_BIT) & CRC_MASK; }
uint32_t        WFD5HeaderParser::LV1ID_TR() const { return (GetWord(GetSize() - 1) >> LV1ID_TR_BIT) & LV1ID_TR_MASK; }
uint32_t        WFD5HeaderParser::DataLength_TR() const { return (GetWord(GetSize() - 1) >> DataLength_TR_BIT) & DataLength_TR_MASK; }
unsigned char   WFD5HeaderParser::XADCAlarm() const { return (GetWord(XADCALARM_WORD) >> XADCALARM_BIT) & XADCALARM_MASK; }
bool            WFD5HeaderParser::EmptyEvent() const { return (GetWord(EMPTYEVENT_WORD) >> EMPTYEVENT_BIT) & EMPTYEVENT_MASK; }
unsigned int    WFD5HeaderParser::MajorRevision() const { return (GetWord(MajorRevision_WORD) >> MajorRevision_BIT) & MajorRevision_MASK; }
unsigned int    WFD5HeaderParser::MinorRevision() const { return (GetWord(MinorRevision_WORD) >> MinorRevision_BIT) & MinorRevision_MASK; }
unsigned int    WFD5HeaderParser::PatchRevision() const { return (GetWord(PatchRevision_WORD) >> PatchRevision_BIT) & PatchRevision_MASK; }
unsigned int    WFD5HeaderParser::NumEnabledChannels() const { 
    // the number of enabled channels is equal to the number of 1's
    // in the binary representation of nChannelsBits
    unsigned int nChannelsBits =(GetWord(ChannelsEnabled_WORD) >> ChannelsEnabled_BIT) & ChannelsEnabled_MASK;
    unsigned int nEnabledChannels = 0;
    for (; nChannelsBits > 0; nChannelsBits >>= 1) {
        nEnabledChannels += nChannelsBits & 0x1;
    }
  return nEnabledChannels;
}

std::ostringstream WFD5HeaderParser::Stream() {
    std::ostringstream oss;
    oss << "\t---> Entering AMCSlotNo: " << AMCNo() << std::endl;
    oss << "\tBoardType: " << BoardType() << std::endl;
    oss << "\tTriggerType: " << TriggerType() << std::endl;
    oss << "\tDataLength: " << DataLength() << std::endl;
    oss << "\tBoardID: " << BoardID() << std::endl;
    oss << "\tBcN: " << BcN() << std::endl;
    oss << "\tOrN: " << OrN() << std::endl;
    oss << "\tLV1ID: " << LV1ID() << std::endl;
    oss << "\tClockCounter: " << ClockCounter() << std::endl;
    oss << "\tXADCAlarms are zero: " << (!XADCAlarm() ? "true" : "false") << std::endl;
    oss << "\tEmptyEvent: " << EmptyEvent() << std::endl;
    oss << "\tEndianness: " << Endianness() << std::endl;
    oss << "\tMajorRevision: " << MajorRevision() << std::endl;
    oss << "\tMinorRevision: " << MinorRevision() << std::endl;
    oss << "\tPatchRevision: " << PatchRevision() << std::endl;
    return oss;
}

void WFD5HeaderParser::Print() {
    std::cout << this->Stream().str();

}
