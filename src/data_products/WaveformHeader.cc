#include "WaveformHeader.hh"

using namespace dataProducts;

WaveformHeader::WaveformHeader()
    : DataProduct(),
    crateNum(0),
    amcNum(0),
    waveformCount(0),
    waveformGap(0),
    channelTag(0),
    waveformIndex(0),
    fillType(0),
    waveformLength(0),
    DDR3Address(0),
    preTriggerLength(0),
    xadcAlarms(0),
    dummyData(0),
    triggerWidth(0),
    timeStamp(0) {}

WaveformHeader::WaveformHeader(unsigned int crateNumArg
    ,unsigned int amcNumArg
    ,unsigned int waveformCountArg
    ,unsigned int waveformGapArg
    ,unsigned int channelTagArg
    ,unsigned int waveformIndexArg
    ,unsigned int fillTypeArg
    ,unsigned int waveformLengthArg
    ,unsigned int DDR3AddressArg
    ,unsigned int preTriggerLengthArg
    ,unsigned char xadcAlarmsArg
    ,bool dummyDataArg
    ,unsigned int triggerWidthArg
    ,uint64_t timeStampArg
    ) : crateNum(crateNumArg)
    ,amcNum(amcNumArg)
    ,waveformCount(waveformCountArg)
    ,waveformGap(waveformGapArg)
    ,channelTag(channelTagArg)
    ,waveformIndex(waveformIndexArg)
    ,fillType(fillTypeArg)
    ,waveformLength(waveformLengthArg)
    ,DDR3Address(DDR3AddressArg)
    ,preTriggerLength(preTriggerLengthArg)
    ,xadcAlarms(xadcAlarmsArg)
    ,dummyData(dummyDataArg)
    ,triggerWidth(triggerWidthArg)
    ,timeStamp(timeStampArg)
{}

            
WaveformHeader::~WaveformHeader() {};

void WaveformHeader::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "WaveformHeader: " << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcNum: " << amcNum << std::endl;
    oss << "    waveformCount: " << waveformCount << std::endl;
    oss << "    waveformGap: " << waveformGap << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << "    fillType: " << fillType << std::endl;
    oss << "    waveformLength: " << waveformLength << std::endl;
    oss << "    DDR3Address: " << DDR3Address << std::endl;
    oss << "    preTriggerLength: " << preTriggerLength << std::endl;
    oss << "    xadcAlarms: " << xadcAlarms << std::endl;
    oss << "    dummyData: " << dummyData << std::endl;
    oss << "    triggerWidth: " << triggerWidth << std::endl;
    oss << "    timeStamp: " << timeStamp << std::endl;

    std::cout << oss.str();
}