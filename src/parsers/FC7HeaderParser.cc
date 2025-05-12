#include "FC7HeaderParser.hh"

using namespace parsers;

FC7HeaderParser::FC7HeaderParser() : Parser() {};

FC7HeaderParser::~FC7HeaderParser() {};

std::unique_ptr<dataProducts::FC7Header> FC7HeaderParser::NewDataProduct(unsigned int crateNum) {
    return std::make_unique<dataProducts::FC7Header>(crateNum
                ,AMCNo()
                ,TrigNum()
                ,TimeStamp()
                ,TriggerType()
                ,BoardType()
                ,FC7Type()
                ,BoardID()
                ,XADCAlarm()
                ,EmptyEvent()
                ,MajorRevision()
                ,MinorRevision()
                ,PatchRevision()
            );
}

//Get methods
uint32_t FC7HeaderParser::AMCNo()               const { return (GetWord(FC7_AMCNo_WORD) >> FC7_AMCNo_BIT) & FC7_AMCNo_MASK; }
uint32_t FC7HeaderParser::TrigNum()             const { return (GetWord(FC7_TrigNum_WORD) >> FC7_TrigNum_BIT) & FC7_TrigNum_MASK; }
uint32_t FC7HeaderParser::BcN()                 const { return (GetWord(FC7_BcN_WORD) >> FC7_BcN_BIT) & FC7_BcN_MASK; }
uint32_t FC7HeaderParser::OrN()                 const { return (GetWord(FC7_OrN_WORD) >> FC7_OrN_BIT) & FC7_OrN_MASK; }
uint64_t FC7HeaderParser::TimeStamp()           const { return ((uint64_t)BcN() << 32) | ((uint64_t)OrN()); }
uint32_t FC7HeaderParser::DataLength()          const { return (GetWord(FC7_DataLength_WORD) >> FC7_DataLength_BIT) & FC7_DataLength_MASK; }
uint32_t FC7HeaderParser::TriggerType()         const { return (GetWord(FC7_TriggerType_WORD) >> FC7_TriggerType_BIT) & FC7_TriggerType_MASK; }
uint32_t FC7HeaderParser::BoardID()             const { return (GetWord(FC7_BoardID_WORD) >> FC7_BoardID_BIT) & FC7_BoardID_MASK; }
uint32_t FC7HeaderParser::BoardType()           const { return (GetWord(FC7_BoardType_WORD) >> FC7_BoardType_BIT) & FC7_BoardType_MASK; }
uint32_t FC7HeaderParser::FC7Type()             const { return (GetWord(FC7_FC7Type_WORD) >> FC7_FC7Type_BIT) & FC7_FC7Type_MASK; }
uint32_t FC7HeaderParser::CRC()                 const { return (GetWord(GetSize() - 1) >> FC7_CRC_BIT) & FC7_CRC_MASK; }
uint32_t FC7HeaderParser::TrigNum_TR()          const { return (GetWord(GetSize() - 1) >> FC7_TrigNum_TR_BIT) & FC7_TrigNum_TR_MASK; }
uint32_t FC7HeaderParser::DataLength_TR()       const { return (GetWord(GetSize() - 1) >> FC7_DataLength_TR_BIT) & FC7_DataLength_TR_MASK; }
unsigned char FC7HeaderParser::XADCAlarm()      const { return (GetWord(FC7_XADCALARM_WORD) >> FC7_XADCALARM_BIT) & FC7_XADCALARM_MASK; }
bool FC7HeaderParser::EmptyEvent()              const { return (GetWord(FC7_EMPTYEVENT_WORD) >> FC7_EMPTYEVENT_BIT) & FC7_EMPTYEVENT_MASK; }
unsigned int FC7HeaderParser::MajorRevision()   const { return (GetWord(FC7_MajorRevision_WORD) >> FC7_MajorRevision_BIT) & FC7_MajorRevision_MASK; }
unsigned int FC7HeaderParser::MinorRevision()   const { return (GetWord(FC7_MinorRevision_WORD) >> FC7_MinorRevision_BIT) & FC7_MinorRevision_MASK; }
unsigned int FC7HeaderParser::PatchRevision()   const { return (GetWord(FC7_PatchRevision_WORD) >> FC7_PatchRevision_BIT) & FC7_PatchRevision_MASK; }

std::ostringstream FC7HeaderParser::Stream() {
    std::ostringstream oss;
    oss << "\t---> Entering AMCSlotNo: " << AMCNo() << std::endl;
    oss << "\tBoardType: " << BoardType() << std::endl;
    oss << "\tFC7Type: " << FC7Type() << std::endl;
    oss << "\tTriggerType: " << TriggerType() << std::endl;
    oss << "\tBoardID: " << BoardID() << std::endl;
    oss << "\tBcN: " << BcN() << std::endl;
    oss << "\tOrN: " << OrN() << std::endl;
    oss << "\tTrigNum: " << TrigNum() << std::endl;
    oss << "\tDataLength: " << DataLength() << std::endl;
    oss << "\tTimeStamp: " << TimeStamp() << std::endl;
    oss << "\tXADCAlarms are zero: " << (!XADCAlarm() ? "true" : "false") << std::endl;
    oss << "\tEmptyEvent: " << EmptyEvent() << std::endl;
    oss << "\tMajorRevision: " << MajorRevision() << std::endl;
    oss << "\tMinorRevision: " << MinorRevision() << std::endl;
    oss << "\tPatchRevision: " << PatchRevision() << std::endl;
    return oss;
}

void FC7HeaderParser::Print() {
    std::cout << this->Stream().str();
}
