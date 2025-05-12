#ifndef FC7UNPACKER_HH
#define FC7UNPACKER_HH

//Custom
#include "Logger.hh"
#include "PayloadUnpacker.hh"
#include "UnpackerHelpers.hh"
#include "FC7HeaderParser.hh"
#include "FC7Header.hh"

namespace unpackers {

    class FC7Unpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        FC7Unpacker();

        //Destructor
        ~FC7Unpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> FC7HeaderPtrCol_;

    private:
        const std::string className_ = "FC7Unpacker";

        //Parser
        std::unique_ptr<parsers::FC7HeaderParser> FC7HeaderParser_;


    };
}

#endif // FC7UNPACKER_HH