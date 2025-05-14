#include "nalu/data_products/NaluEventHeader.hh"

using namespace dataProducts;

NaluEventHeader::NaluEventHeader()
    : DataProduct()
    ,event_header(0)
    ,event_info(0)
    ,event_index(0)
    ,event_reference_time(0)
    ,packet_size(0)
    ,channel_mask(0)
    ,num_windows(0)
    ,num_packets(0)
{}

NaluEventHeader::NaluEventHeader(
     uint16_t event_header_arg
    ,uint16_t event_info_arg
    ,uint32_t event_index_arg
    ,uint32_t event_reference_time_arg
    ,uint16_t packet_size_arg
    ,uint64_t channel_mask_arg
    ,uint16_t num_windows_arg
    ,uint16_t num_packets_arg
    )
    : DataProduct()
    ,event_header(event_header_arg)
    ,event_info(event_info_arg)
    ,event_index(event_index_arg)
    ,event_reference_time(event_reference_time_arg)
    ,packet_size(packet_size_arg)
    ,channel_mask(channel_mask_arg)
    ,num_windows(num_windows_arg)
    ,num_packets(num_packets_arg)
{}

NaluEventHeader::~NaluEventHeader() {};

std::string NaluEventHeader::String() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "NaluEventHeader: " << std::endl;
    oss << "    event_header:          " << std::hex << std::setw(4) << std::setfill('0') << event_header << std::endl;
    oss << "    event_info:            " << std::dec << event_info << std::endl;
    oss << "    event_index:           " << std::dec << event_index << std::endl;
    oss << "    event_reference_time:  " << std::dec << event_reference_time << std::endl;
    oss << "    packet_size:           " << std::dec << packet_size << std::endl;
    oss << "    channel_mask:          " << std::hex << std::setw(16) << std::setfill('0') << channel_mask << std::endl;
    oss << "    num_windows:           " << std::dec << num_windows << std::endl;
    oss << "    num_packets:           " << std::dec << num_packets << std::endl;
    return oss.str();
}

void NaluEventHeader::Show() const { 
    std::cout << this->String();
}