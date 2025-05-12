#include "ChannelHeaderParser.hh"

using namespace parsers;

ChannelHeaderParser::ChannelHeaderParser() : Parser() {};

ChannelHeaderParser::~ChannelHeaderParser() {};

std::unique_ptr<dataProducts::ChannelHeader>
ChannelHeaderParser::NewDataProduct(unsigned int crateNum
                                    ,unsigned int amcNum) {
    return std::make_unique<dataProducts::ChannelHeader>(
                crateNum
                ,amcNum
                ,WaveformCount()
                ,0
                ,ChannelTag()
                ,TrigNum()
                ,FillType()
                ,WaveformLength()
                ,DDR3Address()
                ,!DataIntCheck()
                ,PreTriggerLength()
                ,0
            );
}

uint32_t ChannelHeaderParser::ChannelTag() const             { return (GetWord(ChSync_ChannelTag_WORD) >> ChSync_ChannelTag_BIT) & ChSync_ChannelTag_MASK; }
uint32_t ChannelHeaderParser::PreTriggerLength() const       { return (GetWord(ChSync_PreTrigLength_WORD) >> ChSync_PreTrigLength_BIT) & ChSync_PreTrigLength_MASK; }
uint32_t ChannelHeaderParser::WaveformCount() const          { return (GetWord(ChSync_WaveformCount_WORD) >> ChSync_WaveformCount_BIT) & ChSync_WaveformCount_MASK; }
uint32_t ChannelHeaderParser::DDR3Address_1() const          { return (GetWord(ChSync_DDR3Address_1_WORD) >> ChSync_DDR3Address_1_BIT) & ChSync_DDR3Address_1_MASK; }
uint32_t ChannelHeaderParser::DDR3Address_2() const          { return (GetWord(ChSync_DDR3Address_2_WORD) >> ChSync_DDR3Address_2_BIT) & ChSync_DDR3Address_2_MASK; }
uint32_t ChannelHeaderParser::DDR3Address() const            { return (DDR3Address_2() << 14) | DDR3Address_1(); }
uint32_t ChannelHeaderParser::WaveformLength()  const        { return (GetWord(ChSync_WaveformLength_WORD) >> ChSync_WaveformLength_BIT) & ChSync_WaveformLength_MASK; }
uint32_t ChannelHeaderParser::FillType()  const              { return (GetWord(ChSync_FillType_WORD) >> ChSync_FillType_BIT) & ChSync_FillType_MASK; }
uint32_t ChannelHeaderParser::TrigNum() const                { return (GetWord(ChSync_TrigNum_WORD) >> ChSync_TrigNum_BIT) & ChSync_TrigNum_MASK; }
uint32_t ChannelHeaderParser::DataIntCheck() const           { return (GetTrailerWord(ChSync_DataIntCheck_WORD) >> ChSync_DataIntCheck_BIT) & ChSync_DataIntCheck_MASK; }

std::ostringstream ChannelHeaderParser::Stream() {
    std::ostringstream oss;
    oss << "\t\t---> Entering Channel #" << ChannelTag() << std::endl;
    oss << "\t\tFillType: " << FillType() << std::endl;
    oss << "\t\tPreTriggerLength: " << PreTriggerLength() << std::endl;
    oss << "\t\tWaveformCount: " << WaveformCount() << std::endl;
    oss << "\t\tWaveformLength: " << WaveformLength() << std::endl;
    oss << "\t\tTrigNum: " << TrigNum() << std::endl;
    oss << "\t\tDDR3Address: 0x" << std::hex << DDR3Address() << std::dec << std::endl;
    return oss;
}

void ChannelHeaderParser::Print() {
    std::cout << this->Stream().str();
}