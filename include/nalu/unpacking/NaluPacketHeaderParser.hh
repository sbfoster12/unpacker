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

        DataLocation packet_header_data_location_;
        DataLocation packet_info_data_location_;
        DataLocation channel_data_location_;
        DataLocation trigger_time_data_location_;
        DataLocation logical_position_data_location_;
        DataLocation window_position_data_location_;

    };
}

#endif // NALUPACKETHEADERPARSER_HH