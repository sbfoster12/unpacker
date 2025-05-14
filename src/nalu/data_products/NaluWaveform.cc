#include "nalu/data_products/NaluWaveform.hh"
#include <numeric>

using namespace dataProducts;

NaluWaveform::NaluWaveform()
    : DataProduct()
{}

NaluWaveform::NaluWaveform(NaluPacketCollection nalu_packets
    ) : DataProduct()
{

    if (nalu_packets.size() != 0) {
        channel_num = nalu_packets.at(0).channel_num;
    }

    for (const auto& nalu_packet : nalu_packets) {
        trace.insert(trace.end(),nalu_packet.trace.begin(),nalu_packet.trace.end());
    }
}

NaluWaveform::~NaluWaveform() {}

std::string NaluWaveform::String() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "NaluWaveform:" << std::endl;
    oss << "    channel_num: " << channel_num << std::endl;
    oss << "    trace: ";
    for (const auto & sample : trace) {
        oss << sample << ", ";
    }
    oss << std::endl;
    return oss.str();
}

void NaluWaveform::Show() const { 
    std::cout << this->String();
}
