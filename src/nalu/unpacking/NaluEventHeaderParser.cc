#include "nalu/unpacking/NaluEventHeaderParser.hh"

using namespace parsers;

NaluEventHeaderParser::NaluEventHeaderParser() : Parser() {};

NaluEventHeaderParser::~NaluEventHeaderParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluEventHeader> 
NaluEventHeaderParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluEventHeader>(
                 GetEventHeader()
                ,GetEventInfo()
                ,GetEventIndex()
                ,GetEventReferenceTime()
                ,GetPacketSize()
                ,GetChannelMask()
                ,GetNumWindows()
                ,GetNumPackets()
            );
}

//Get methods
uint16_t    NaluEventHeaderParser::GetEventHeader() const { return GetWord(0) & 0xffff; }
uint16_t    NaluEventHeaderParser::GetEventInfo() const { return (GetWord(0) >> 16) & 0xff; }
uint32_t    NaluEventHeaderParser::GetEventIndex() const { return (GetWord(0) >> 24) & 0xffffffff; }
uint32_t    NaluEventHeaderParser::GetEventReferenceTime() const { uint64_t part1 = (GetWord(0) >> 56) & 0xff; uint64_t part2 =  GetWord(1) & 0xffffff; return part1 | (part2 << 8); }
uint16_t    NaluEventHeaderParser::GetPacketSize() const { return (GetWord(1) >> 24) & 0xffff; }
uint64_t    NaluEventHeaderParser::GetChannelMask() const { uint64_t part1 = (GetWord(1) >> 40) & 0xffffff; uint64_t part2 =  GetWord(2) & 0xffffffffff; return part1 | (part2 << 24);  }
uint16_t    NaluEventHeaderParser::GetNumWindows() const { return (GetWord(2) >> 40) & 0xff; }
uint16_t    NaluEventHeaderParser::GetNumPackets() const { return (GetWord(2) >> 48) & 0xffff; }

std::ostringstream NaluEventHeaderParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering NaluEventHeader: " << std::endl;
    oss << "            EventHeader: " << "0x" << std::hex << std::setw(4) << std::setfill('0') << GetEventHeader() << std::endl;
    oss << "            EventInfo: " << std::dec << GetEventInfo() << std::endl;
    oss << "            EventIndex: "<< std::dec << GetEventIndex() << std::endl;
    oss << "            EventReferenceTime: " << std::dec << GetEventReferenceTime() << std::endl;
    oss << "            PacketSize: " << std::dec << GetPacketSize() << std::endl;
    oss << "            ChannelMask: " << "0x" <<  std::hex << std::setw(16) << std::setfill('0') << GetChannelMask() << std::endl;
    oss << "            NumWindows: " << std::dec << GetNumWindows() << std::endl;
    oss << "            NumPackets: " << std::dec << GetNumPackets() << std::endl;
    return oss;
}

void NaluEventHeaderParser::Print() {
    std::cout << this->Stream().str();

}
