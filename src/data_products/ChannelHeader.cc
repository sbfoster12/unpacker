#include "ChannelHeader.hh"

using namespace dataProducts;

ChannelHeader::ChannelHeader()
    : DataProduct(),
    crateNum(0),
    amcNum(0),
    waveformCount(0),
    waveformGap(0),
    channelTag(0),
    triggerNum(0),
    fillType(0),
    waveformLength(0),
    DDR3Address(0),
    passedIntegrityCheck(false),
    preTriggerLength(0),
    eventLength(0)
{};

ChannelHeader::ChannelHeader(unsigned int crateNumArg
    ,unsigned int amcNumArg
    ,unsigned int waveformCountArg
    ,unsigned int waveformGapArg
    ,unsigned int channelTagArg
    ,unsigned int triggerNumArg
    ,unsigned int fillTypeArg
    ,unsigned int waveformLengthArg
    ,unsigned int DDR3AddressArg
    ,bool passedArg
    ,unsigned int preTriggerLengthArg
    ,unsigned int eventLengthArg
    ) : crateNum(crateNumArg)
    ,amcNum(amcNumArg)
    ,waveformCount(waveformCountArg)
    ,waveformGap(waveformGapArg)
    ,channelTag(channelTagArg)
    ,triggerNum(triggerNumArg)
    ,fillType(fillTypeArg)
    ,waveformLength(waveformLengthArg)
    ,DDR3Address(DDR3AddressArg)
    ,passedIntegrityCheck(passedArg)
    ,preTriggerLength(preTriggerLengthArg)
    ,eventLength(eventLengthArg)
{}

ChannelHeader::~ChannelHeader() {};

void ChannelHeader::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "ChannelHeader:" << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcNum: " << amcNum << std::endl;
    oss << "    waveformCount: " << waveformCount << std::endl;
    oss << "    waveformGap: " << waveformGap << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    triggerNum: " << triggerNum << std::endl;
    oss << "    fillType: " << fillType << std::endl;
    oss << "    waveformLength: " << waveformLength << std::endl;
    oss << "    DDR3Address: " << DDR3Address << std::endl;
    oss << "    passedIntegrityCheck: " << (passedIntegrityCheck ? "true" : "false") << std::endl;
    oss << "    preTriggerLength: " << preTriggerLength << std::endl;
    oss << "    eventLength: " << eventLength << std::endl;
    std::cout << oss.str();
}