#ifndef NALUPACKETPARSER_HH
#define NALUPACKETPARSER_HH

//Custom
#include "common/unpacking/Parser.hh"
#include "nalu/data_products/NaluPacket.hh"

namespace parsers {

    class NaluPacketParser : public Parser {
        
    public:
        
        //Constructor
        NaluPacketParser();

        //Destructor
        ~NaluPacketParser();

        //Method to create shared ptr
        dataProducts::NaluPacket NewDataProduct(uint64_t channel_num, uint64_t window_position);

        //Get methods
        std::vector<short> GetTrace() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluPacketParser";

    };
}

#endif // NALUPACKETPARSER_HH