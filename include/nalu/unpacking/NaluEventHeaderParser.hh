#ifndef NALUEVENTHEADERPARSER_HH
#define NALUEVENTHEADERPARSER_HH

//Custom
#include "common/unpacking/Parser.hh"
#include "nalu/data_products/NaluEventHeader.hh"

namespace parsers {

    class NaluEventHeaderParser : public Parser {
        
    public:
        
        //Constructor
        NaluEventHeaderParser();

        //Destructor
        ~NaluEventHeaderParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluEventHeader> NewDataProduct();

        //Get methods
        uint16_t    GetEventHeader() const;
        uint16_t    GetEventInfo() const;
        uint32_t    GetEventIndex() const;
        uint32_t    GetEventReferenceTime() const;
        uint16_t    GetPacketSize() const;
        uint64_t    GetChannelMask() const;
        uint16_t    GetNumWindows() const;
        uint16_t    GetNumPackets() const;


        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluEventHeaderParser";

    };
}

#endif // NALUEVENTHEADERPARSER_HH