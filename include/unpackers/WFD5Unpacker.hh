#ifndef WFD5UNPACKER_HH
#define WFD5UNPACKER_HH

//Custom
#include "WFD5Header.hh"
#include "Waveform.hh"

#include "Logger.hh"
#include "PayloadUnpacker.hh"
#include "UnpackerHelpers.hh"
#include "WFD5HeaderParser.hh"
#include "ChannelHeaderParser.hh"
#include "WaveformHeaderParser.hh"
#include "WaveformParser.hh"
#include "ChannelHeaderAsyncParser.hh"
#include "WaveformHeaderAsyncParser.hh"

namespace unpackers {

    class WFD5Unpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        WFD5Unpacker();

        //Destructor
        ~WFD5Unpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Methods to unpack channels in sync and async modes
        void UnpackSyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot);
        void UnpackASyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot);

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> WFD5HeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> ChannelHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> WaveformHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> WaveformPtrCol_;

    private:
        const std::string className_ = "WFD5Unpacker";

        //Parsers
        std::unique_ptr<parsers::WFD5HeaderParser> WFD5HeaderParser_;
        std::unique_ptr<parsers::ChannelHeaderParser> ChannelHeaderParser_;
        std::unique_ptr<parsers::WaveformHeaderParser> WaveformHeaderParser_;
        std::unique_ptr<parsers::WaveformParser> WaveformParser_;
        //Async mode
        std::unique_ptr<parsers::ChannelHeaderAsyncParser> ChannelHeaderAsyncParser_;
        std::unique_ptr<parsers::WaveformHeaderAsyncParser> WaveformHeaderAsyncParser_;

        unsigned long clockCounter_; //40 MHz TTC trigger time stamp

    };
}

#endif // WFD5UNPACKER_HH