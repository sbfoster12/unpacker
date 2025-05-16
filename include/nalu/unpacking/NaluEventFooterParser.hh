#ifndef NALUEVENTFOOTERPARSER_HH
#define NALUEVENTFOOTERPARSER_HH

//Custom
#include "common/unpacking/Parser.hh"
#include "nalu/data_products/NaluEventFooter.hh"

namespace parsers {

    class NaluEventFooterParser : public Parser {
        
    public:
        
        //Constructor
        NaluEventFooterParser();

        //Destructor
        ~NaluEventFooterParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluEventFooter> NewDataProduct();

        //Get methods
        uint32_t        GetEventFooter() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluEventFooterParser";

        DataLocation event_footer_data_location_;

    };
}

#endif // NALUEVENTFOOTERPARSER_HH