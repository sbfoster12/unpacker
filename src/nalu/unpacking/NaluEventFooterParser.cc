#include "nalu/unpacking/NaluEventFooterParser.hh"

using namespace parsers;

NaluEventFooterParser::NaluEventFooterParser() : Parser() {};

NaluEventFooterParser::~NaluEventFooterParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluEventFooter> 
NaluEventFooterParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluEventFooter>(
                GetEventFooter()
            );
}

//Get methods
uint32_t        NaluEventFooterParser::GetEventFooter() const { return GetWord(0) & 0xffffffff; }

std::ostringstream NaluEventFooterParser::Stream() {
    std::ostringstream oss;
    oss << "   ---> Entering NaluEventFooter: " << std::endl;
    oss << "            EventFooter: " << std::hex << "0x" << GetEventFooter() << std::endl;
    return oss;
}

void NaluEventFooterParser::Print() {
    std::cout << this->Stream().str();

}
