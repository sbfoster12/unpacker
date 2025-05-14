#include "nalu/data_products/NaluPacketFooter.hh"

using namespace dataProducts;

NaluPacketFooter::NaluPacketFooter()
    : DataProduct(),
    parser_index(0),
    packet_footer(0)
{}

NaluPacketFooter::NaluPacketFooter(
    uint32_t parser_index_arg,
    uint32_t packet_footer_arg
    )
    : DataProduct(),
    parser_index(parser_index_arg),
    packet_footer(packet_footer_arg)
{}

NaluPacketFooter::~NaluPacketFooter() {};

std::string NaluPacketFooter::String() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "NaluPacketFooter: " << std::endl;
    oss << "    parser_index: " << std::dec << parser_index << std::endl;
    oss << "    packet_footer: " << std::hex << "0x" << packet_footer << std::endl;
    return oss.str();
}

void NaluPacketFooter::Show() const { 
    std::cout << this->String();
}