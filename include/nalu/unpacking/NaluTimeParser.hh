#ifndef NALUTIMEPARSER_HH
#define NALUTIMEPARSER_HH

//Custom
#include "common/unpacking/Parser.hh"
#include "nalu/data_products/NaluTime.hh"

namespace parsers {

    class NaluTimeParser : public Parser {
        
    public:
        
        //Constructor
        NaluTimeParser();

        //Destructor
        ~NaluTimeParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluTime> NewDataProduct();

        //Get methods
        uint64_t    GetCollectionCycleIndex() const;
        uint64_t    GetCollectionCycleTimeStampNs() const;
        double    GetUDPTime() const;
        double    GetParseTime() const;
        double    GetEventTime() const;
        double    GetTotalTime() const;
        uint64_t    GetDataProcessed() const;
        double    GetDataRate() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluTimeParser";

    };
}

#endif // NALUTIMEPARSER_HH