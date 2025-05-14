#ifndef ADPAYLOADUNPACKER_HH
#define ADPAYLOADUNPACKER_HH

#include "common/unpacking/Logger.hh"
#include "common/unpacking/PayloadUnpacker.hh"
#include "common/unpacking/UnpackerHelpers.hh"
#include "nalu/unpacking/NaluEventHeaderParser.hh"
#include "nalu/unpacking/NaluPacketHeaderParser.hh"
#include "nalu/unpacking/NaluPacketParser.hh"
#include "nalu/unpacking/NaluPacketFooterParser.hh"
#include "nalu/unpacking/NaluEventFooterParser.hh"
#include "nalu/data_products/NaluEventHeader.hh"
#include "nalu/data_products/NaluPacketHeader.hh"
#include "nalu/data_products/NaluPacket.hh"
#include "nalu/data_products/NaluWaveform.hh"
#include "nalu/data_products/NaluPacketFooter.hh"
#include "nalu/data_products/NaluEventFooter.hh"


namespace unpackers {

    class ADPayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        ADPayloadUnpacker();

        //Destructor
        ~ADPayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluEventHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluPacketHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluPacketFooterPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluWaveformPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluEventFooterPtrCol_;

    private:
        const std::string className_ = "ADPayloadUnpacker";

        //Parsers
        std::unique_ptr<parsers::NaluEventHeaderParser> naluEventHeaderParser_;
        std::unique_ptr<parsers::NaluPacketHeaderParser> naluPacketHeaderParser_;
        std::unique_ptr<parsers::NaluPacketParser> naluPacketParser_;
        std::unique_ptr<parsers::NaluPacketFooterParser> naluPacketFooterParser_;
        std::unique_ptr<parsers::NaluEventFooterParser> naluEventFooterParser_;

    };
}

#endif // ADPAYLOADUNPACKER_HH