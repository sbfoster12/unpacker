#ifndef WAVEFORMPARSER_HH
#define WAVEFORMPARSER_HH

//Custom
#include "Parser.hh"
#include "Waveform.hh"

// Bit locations and masks for WFD5 waveform format:

namespace parsers {

    class WaveformParser : public Parser {
        
    public:
        
        //Constructor
        WaveformParser();

        //Destructor
        ~WaveformParser();

        //Get methods
        std::vector<short>  ADCData() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

    private:
        std::string className_ = "WaveformParser";

    };
}

#endif // WAVEFORMPARSER_HH