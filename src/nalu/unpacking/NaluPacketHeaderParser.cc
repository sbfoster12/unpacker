#include "nalu/unpacking/NaluPacketHeaderParser.hh"

using namespace parsers;

NaluPacketHeaderParser::NaluPacketHeaderParser() : Parser() {};

NaluPacketHeaderParser::~NaluPacketHeaderParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluPacketHeader> 
NaluPacketHeaderParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluPacketHeader>(
                 GetPacketHeader()
                ,GetPacketInfo()
                ,GetChannel()
                ,GetTriggerTime()
                ,GetLogicalPosition()
                ,GetWindowPosition()
            );
}

//Get methods
uint16_t            NaluPacketHeaderParser::GetPacketHeader() const { return (GetWord(0) & 0xffff); }
uint16_t            NaluPacketHeaderParser::GetPacketInfo() const { return (GetWord(0) >> 16) & 0xff; }
uint16_t            NaluPacketHeaderParser::GetChannel() const { return (GetWord(0) >> 24) & 0xff; }
uint32_t            NaluPacketHeaderParser::GetTriggerTime() const { return (GetWord(0) >> 32) & 0xffffffff; }
uint16_t            NaluPacketHeaderParser::GetLogicalPosition() const { return GetWord(1) & 0xffff; }
uint16_t            NaluPacketHeaderParser::GetWindowPosition() const { return (GetWord(1) >> 16) & 0xffff; }

std::ostringstream NaluPacketHeaderParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering NaluPacketHeader: " << std::endl;
    oss << "            PacketHeader: " << "0x" << std::hex << std::setw(4) << std::setfill('0') << GetPacketHeader() << std::endl;
    oss << "            PacketInfo: " << std::dec << GetPacketInfo() << std::endl;
    oss << "            Channel: " << std::dec << GetChannel() << std::endl;
    oss << "            TriggerTime: " << std::dec << GetTriggerTime() << std::endl;
    oss << "            LogicalPosition: " << std::dec << GetLogicalPosition() << std::endl;
    oss << "            WindowPosition: " << std::dec << GetWindowPosition() << std::endl;
    return oss;
}

void NaluPacketHeaderParser::Print() {
    std::cout << this->Stream().str();

}
