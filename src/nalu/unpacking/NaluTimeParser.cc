#include "nalu/unpacking/NaluTimeParser.hh"

using namespace parsers;

NaluTimeParser::NaluTimeParser() : Parser() {};

NaluTimeParser::~NaluTimeParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluTime> 
NaluTimeParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluTime>(
                 GetCollectionCycleIndex()
                ,GetCollectionCycleTimeStampNs()
                ,GetUDPTime()
                ,GetParseTime()
                ,GetEventTime()
                ,GetTotalTime()
                ,GetDataProcessed()
                ,GetDataRate()
            );
}

//Get methods
uint64_t    NaluTimeParser::GetCollectionCycleIndex() const { return GetWord(0); }
uint64_t    NaluTimeParser::GetCollectionCycleTimeStampNs() const { return GetWord(1); }
double      NaluTimeParser::GetUDPTime() const { uint64_t word = GetWord(2); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      NaluTimeParser::GetParseTime() const{ uint64_t word = GetWord(3); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      NaluTimeParser::GetEventTime() const { uint64_t word = GetWord(4); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      NaluTimeParser::GetTotalTime() const { uint64_t word = GetWord(5); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
uint64_t    NaluTimeParser::GetDataProcessed() const { return GetWord(6); }
double      NaluTimeParser::GetDataRate() const { uint64_t word = GetWord(7); double result; std::memcpy(&result, &word, sizeof(double)); return result; }


std::ostringstream NaluTimeParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering NaluTime: " << std::endl;
    oss << "            CollectionCycleIndex: " << GetCollectionCycleIndex() << std::endl;
    oss << "            CollectionCycleTimeStampNs: " << GetCollectionCycleTimeStampNs() << std::endl;
    oss << "            UDPTime: " << GetUDPTime() << std::endl;
    oss << "            ParseTime " << GetParseTime() << std::endl;
    oss << "            EventTime: " << GetEventTime() << std::endl;
    oss << "            TotalTime: " << GetTotalTime() << std::endl;
    oss << "            DataProcessed: " << GetDataProcessed() << std::endl;
    oss << "            DataRate: " << GetDataRate() << std::endl;
    return oss;
}

void NaluTimeParser::Print() {
    std::cout << this->Stream().str();

}
