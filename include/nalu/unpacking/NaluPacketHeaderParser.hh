#ifndef NALUPACKETHEADERPARSER_HH
#define NALUPACKETHEADERPARSER_HH

//Custom
#include "common/unpacking/Parser.hh"
#include "nalu/data_products/NaluPacketHeader.hh"

namespace parsers {

    class NaluPacketHeaderParser : public Parser {
        
    public:
        
        //Constructor
        NaluPacketHeaderParser();

        //Destructor
        ~NaluPacketHeaderParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluPacketHeader> NewDataProduct();

        //Get methods
        uint16_t          GetPacketHeader() const;
        uint16_t          GetPacketInfo() const;
        uint16_t          GetChannel() const;
        uint32_t          GetTriggerTime() const;
        uint16_t          GetLogicalPosition() const;
        uint16_t          GetWindowPosition() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluPacketHeaderParser";

    };
}

#endif // NALUPACKETHEADERPARSER_HH