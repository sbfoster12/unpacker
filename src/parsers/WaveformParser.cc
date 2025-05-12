#include "WaveformParser.hh"

using namespace parsers;

WaveformParser::WaveformParser() : Parser() {};

WaveformParser::~WaveformParser() {};

std::vector<short> WaveformParser::ADCData() const {
    //initialize output vector
    std::vector<short> trace = {};
    //loop over each word
    for (const uint64_t& word : words_) {
        //cast to a short
        short* shortWords = (short*)&word;
        //insert into vector
        size_t wordsSize = sizeof(word)/sizeof(short);
        trace.insert(trace.end(),shortWords, shortWords + wordsSize);
    } 
    return trace;
}

std::ostringstream WaveformParser::Stream() {
    std::ostringstream oss;
    oss << "\t\t\t\t---> Entering Waveform data: " << std::endl;
    oss << "\t\t\t\tADCData.size(): " << ADCData().size() << std::endl;
    return oss;
}

void WaveformParser::Print() {
    std::cout << this->Stream().str();
}
