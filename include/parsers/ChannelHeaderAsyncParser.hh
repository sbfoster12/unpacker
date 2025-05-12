#ifndef CHANNELHEADERASYNCPARSER_HH
#define CHANNELHEADERASYNCPARSER_HH

//Custom
#include "Parser.hh"
#include "ChannelHeader.hh"

// Bit locations and masks for WFD5 channel format:
#define ChAsync_WaveformCount_WORD 0
#define ChAsync_WaveformCount_BIT 16
#define ChAsync_WaveformCount_MASK 0x7fffff
#define ChAsync_ChannelTag_WORD 0
#define ChAsync_ChannelTag_BIT 46
#define ChAsync_ChannelTag_MASK 0xfff
#define ChAsync_PreTrigLength_WORD 0
#define ChAsync_PreTrigLength_BIT 0
#define ChAsync_PreTrigLength_MASK 0xffff
#define ChAsync_WaveformLength_WORD 1
#define ChAsync_WaveformLength_BIT 50
#define ChAsync_WaveformLength_MASK 0x3fff
#define ChAsync_EventLength_WORD 1
#define ChAsync_EventLength_BIT 27
#define ChAsync_EventLength_MASK 0x7fffff
#define ChAsync_TrigNum_WORD 1
#define ChAsync_TrigNum_BIT 0
#define ChAsync_TrigNum_MASK 0xffffff
#define ChAsync_FillType_WORD 1
#define ChAsync_FillType_BIT 24
#define ChAsync_FillType_MASK 0x7
#define ChAsync_DataIntCheck_WORD 2
#define ChAsync_DataIntCheck_BIT 32
#define ChAsync_DataIntCheck_MASK 0xffffffff

namespace parsers {

    class ChannelHeaderAsyncParser : public Parser {
        
    public:
        
        //Constructor
        ChannelHeaderAsyncParser();

        //Destructor
        ~ChannelHeaderAsyncParser();

        //Method to make the data product
        std::unique_ptr<dataProducts::ChannelHeader>
        NewDataProduct(unsigned int crateNum
                    ,unsigned int amcNum);

        uint32_t WaveformCount() const;
        uint32_t ChannelTag() const;
        uint32_t PreTriggerLength() const;
        uint32_t WaveformLength() const;
        uint32_t EventLength() const;
        uint32_t TrigNum() const;
        uint32_t FillType() const;
        uint32_t DataIntCheck() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "ChannelHeaderAsyncParser";

    };
}

#endif // CHANNELHEADERASYNCPARSER_HH