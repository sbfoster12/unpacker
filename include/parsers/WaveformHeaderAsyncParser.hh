#ifndef WAVEFORMHEADERASYNCPARSER_HH
#define WAVEFORMHEADERASYNCPARSER_HH

//Custom
#include "Parser.hh"
#include "WaveformHeader.hh"

// Bit locations and masks for waveform header format:
#define WFAsync_WaveformIndex1_WORD 0
#define WFAsync_WaveformIndex1_BIT 52
#define WFAsync_WaveformIndex1_MASK 0xfff
#define WFAsync_WaveformIndex2_WORD 1
#define WFAsync_WaveformIndex2_BIT 0
#define WFAsync_WaveformIndex2_MASK 0x7ff
#define WFAsync_DDR3Address_WORD 0
#define WFAsync_DDR3Address_BIT 26
#define WFAsync_DDR3Address_MASK 0x7fffff
#define WFAsync_PreTriggerLength_WORD 0
#define WFAsync_PreTriggerLength_BIT 14
#define WFAsync_PreTriggerLength_MASK 0xfff
#define WFAsync_WaveformLength_WORD 0
#define WFAsync_WaveformLength_BIT 0
#define WFAsync_WaveformLength_MASK 0x3fff
#define WFAsync_XADC_WORD 1
#define WFAsync_XADC_BIT 58
#define WFAsync_XADC_MASK 0xf
#define WFAsync_D_WORD 1
#define WFAsync_D_BIT 57
#define WFAsync_D_MASK 0x1
#define WFAsync_TS_WORD 1
#define WFAsync_TS_BIT 55
#define WFAsync_TS_MASK 0x3
#define WFAsync_Timestamp_WORD 1
#define WFAsync_Timestamp_BIT 11
#define WFAsync_Timestamp_MASK 0xfffffffffff

namespace parsers {

    class WaveformHeaderAsyncParser : public Parser {
        
    public:
        
        //Constructor
        WaveformHeaderAsyncParser();

        //Destructor
        ~WaveformHeaderAsyncParser();

        //Method to make the data product
        std::unique_ptr<dataProducts::WaveformHeader>
        NewDataProduct(unsigned int crateNum
                    ,unsigned int amcNum); 

        //Get methods
        uint32_t WaveformIndex() const;
        uint32_t WaveformIndex1() const;
        uint32_t WaveformIndex2() const;
        uint32_t DDR3Address() const;
        uint32_t PreTriggerLength() const;
        uint32_t WaveformLength() const;
        uint32_t XADCAlarm() const;
        uint32_t D() const;
        uint32_t TS() const;
        uint64_t ClockCounter() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "WaveformHeaderAsyncParser";

    };
}

#endif // WAVEFORMHEADERASYNCPARSER_HH