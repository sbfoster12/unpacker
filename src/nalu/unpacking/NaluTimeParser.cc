#include "nalu/unpacking/NaluTimeParser.hh"

using namespace parsers;
using json = nlohmann::json;

NaluTimeParser::NaluTimeParser() : Parser() {

    //Set each data location to its default values
    collection_cycle_index_data_location_ =             DataLocation(0,0,8);
    collection_cycle_time_stamp_ns_data_location_ =     DataLocation(1,0,8);
    udp_time_data_location_ =                           DataLocation(2,0,8);
    parse_time_data_location_ =                         DataLocation(3,0,8);
    event_time_data_location_ =                         DataLocation(4,0,8);
    total_time_data_location_ =                         DataLocation(5,0,8);
    data_processed_data_location_ =                     DataLocation(6,0,8);
    data_rate_data_location_ =                          DataLocation(7,0,8);

    //Now look for a configuration file and use that

    // Get the path to the data configuration file
    // First get the unpacker path
    const char* unpacker_path = std::getenv("UNPACKER_PATH");
    if (!unpacker_path) {
        utils::LoggerHolder::getInstance().WarningLogger << "In " << className_ << " UNPACKER_PATH not set! Using default values for data location instead.\n" << std::endl;
    } else {

        std::string base_path(unpacker_path);
        std::string file_name = "config/nalu_data_config.json";

        // Make sure base_path ends with a slash '/' or add one
        if (!base_path.empty() && base_path.back() != '/') {
            base_path += '/';
        }

        std::string full_path = base_path + file_name;

        std::ifstream file(full_path);
        if (!file) { 
            utils::LoggerHolder::getInstance().WarningLogger << "In " << className_ << " could not open config file here: " << full_path << ". Using default values for data location instead." << std::endl;
        } else {

            // Make a json object from the input file
            json j;
            file >> j;

            // Access the top-level section first
            const auto& nalu_time = GetSection(j, "nalu_time");

            collection_cycle_index_data_location_ = ParseJsonData(GetSection(nalu_time, "collection_cycle_index"));
            collection_cycle_time_stamp_ns_data_location_ = ParseJsonData(GetSection(nalu_time, "collection_cycle_time_stamps_ns"));
            udp_time_data_location_ = ParseJsonData(GetSection(nalu_time, "udp_time"));
            parse_time_data_location_ = ParseJsonData(GetSection(nalu_time, "parse_time"));
            event_time_data_location_ = ParseJsonData(GetSection(nalu_time, "event_time"));
            total_time_data_location_ = ParseJsonData(GetSection(nalu_time, "total_time"));
            data_processed_data_location_ = ParseJsonData(GetSection(nalu_time, "data_processed"));
            data_rate_data_location_ = ParseJsonData(GetSection(nalu_time, "data_rate"));
        }
    }
};

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
uint64_t    NaluTimeParser::GetCollectionCycleIndex()           const { return GetData(collection_cycle_index_data_location_); }
uint64_t    NaluTimeParser::GetCollectionCycleTimeStampNs()     const { return GetData(collection_cycle_time_stamp_ns_data_location_); }
double      NaluTimeParser::GetUDPTime()                        const { uint64_t word = GetData(udp_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      NaluTimeParser::GetParseTime()                      const { uint64_t word = GetData(parse_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      NaluTimeParser::GetEventTime()                      const { uint64_t word = GetData(event_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      NaluTimeParser::GetTotalTime()                      const { uint64_t word = GetData(total_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
uint64_t    NaluTimeParser::GetDataProcessed()                  const { return GetData(data_processed_data_location_); }
double      NaluTimeParser::GetDataRate()                       const { uint64_t word = GetData(data_rate_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }


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
