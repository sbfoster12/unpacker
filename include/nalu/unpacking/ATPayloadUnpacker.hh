#ifndef ATPAYLOADUNPACKER_HH
#define ATPAYLOADUNPACKER_HH

#include "common/unpacking/Logger.hh"
#include "common/unpacking/PayloadUnpacker.hh"
#include "common/unpacking/UnpackerHelpers.hh"
#include "nalu/unpacking/NaluTimeParser.hh"
#include "nalu/data_products/NaluTime.hh"


namespace unpackers {

    class ATPayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        ATPayloadUnpacker();

        //Destructor
        ~ATPayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluTimePtrCol_;

    private:
        const std::string className_ = "ATPayloadUnpacker";

        //Parsers
        std::unique_ptr<parsers::NaluTimeParser> naluTimeParser_;

    };
}

#endif // ATPAYLOADUNPACKER_HH