#include "nalu/unpacking/NaluPacketFooterParser.hh"

using namespace parsers;

NaluPacketFooterParser::NaluPacketFooterParser() : Parser() {};

NaluPacketFooterParser::~NaluPacketFooterParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluPacketFooter> 
NaluPacketFooterParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluPacketFooter>(
                GetParserIndex(),
                GetPacketFooter()
            );
}

//Get methods
uint32_t        NaluPacketFooterParser::GetParserIndex() const { return (GetWord(0) >> 32) & 0xffff; }
uint32_t        NaluPacketFooterParser::GetPacketFooter() const { return (GetWord(0) >> 48) & 0xffff; }

std::ostringstream NaluPacketFooterParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering NaluPacketFooter:" << std::endl;
    oss << "            ParserIndex: " << std::dec << GetParserIndex() << std::endl;
    oss << "            PacketFooter: " << std::hex << "0x" << GetPacketFooter() << std::endl;
    return oss;
}

void NaluPacketFooterParser::Print() {
    std::cout << this->Stream().str();

}
