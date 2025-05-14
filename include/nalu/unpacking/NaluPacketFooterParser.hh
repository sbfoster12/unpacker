#ifndef NALUPACKETFOOTERERPARSER_HH
#define NALUPACKETFOOTERERPARSER_HH

//Custom
#include "common/unpacking/Parser.hh"
#include "nalu/data_products/NaluPacketFooter.hh"

namespace parsers {

    class NaluPacketFooterParser : public Parser {
        
    public:
        
        //Constructor
        NaluPacketFooterParser();

        //Destructor
        ~NaluPacketFooterParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluPacketFooter> NewDataProduct();

        //Get methods
        uint32_t        GetParserIndex() const;
        uint32_t        GetPacketFooter() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluPacketFooterParser";

    };
}

#endif // NALUPACKETFOOTERERPARSER_HH