#ifndef CHANNELHEADERPARSER_HH
#define CHANNELHEADERPARSER_HH

//Custom
#include "Parser.hh"
#include "ChannelHeader.hh"

// Bit locations and masks for WFD5 channel format:
#define ChSync_ChannelTag_WORD 0
#define ChSync_ChannelTag_BIT 46
#define ChSync_ChannelTag_MASK 0xfff
#define ChSync_PreTrigLength_WORD 0
#define ChSync_PreTrigLength_BIT 24
#define ChSync_PreTrigLength_MASK 0xffff
#define ChSync_WaveformCount_WORD 0
#define ChSync_WaveformCount_BIT 12
#define ChSync_WaveformCount_MASK 0xfff
#define ChSync_DDR3Address_1_WORD 1
#define ChSync_DDR3Address_1_BIT 50
#define ChSync_DDR3Address_1_MASK 0x3fff
#define ChSync_DDR3Address_2_WORD 0
#define ChSync_DDR3Address_2_BIT 0
#define ChSync_DDR3Address_2_MASK 0xfff
#define ChSync_WaveformLength_WORD 1
#define ChSync_WaveformLength_BIT 27
#define ChSync_WaveformLength_MASK 0x7fffff
#define ChSync_FillType_WORD 1
#define ChSync_FillType_BIT 24
#define ChSync_FillType_MASK 0x7
#define ChSync_TrigNum_WORD 1
#define ChSync_TrigNum_BIT 0
#define ChSync_TrigNum_MASK 0xffffff
#define ChSync_DataIntCheck_WORD 2
#define ChSync_DataIntCheck_BIT 32
#define ChSync_DataIntCheck_MASK 0xffffffff

namespace parsers {

    class ChannelHeaderParser : public Parser {
        
    public:
        
        //Constructor
        ChannelHeaderParser();

        //Destructor
        ~ChannelHeaderParser();

        //Method to make the data product
        std::unique_ptr<dataProducts::ChannelHeader>
        NewDataProduct(unsigned int crateNum
                    ,unsigned int amcNum);

        uint32_t ChannelTag() const;
        uint32_t PreTriggerLength() const;
        uint32_t WaveformCount() const;
        uint32_t DDR3Address_1() const;
        uint32_t DDR3Address_2() const;
        uint32_t DDR3Address() const;
        uint32_t WaveformLength() const;
        uint32_t FillType() const;
        uint32_t TrigNum() const;
        uint32_t DataIntCheck() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "ChannelHeaderParser";

    };
}

#endif // CHANNELHEADERPARSER_HH