#include "nalu/unpacking/NaluPacketHeaderParser.hh"

using namespace parsers;
using json = nlohmann::json;

NaluPacketHeaderParser::NaluPacketHeaderParser() : Parser() {

     //Set each data location to its default values
    packet_header_data_location_ =              DataLocation(0,0,2);
    packet_info_data_location_ =                DataLocation(0,16,1);
    channel_data_location_ =                    DataLocation(0,24,1);
    trigger_time_data_location_ =               DataLocation(0,32,4);
    logical_position_data_location_ =           DataLocation(1,0,2);
    window_position_data_location_ =            DataLocation(1,16,2);

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
            const auto& nalu_packet_header = GetSection(j, "nalu_packet_header");

            packet_header_data_location_ = ParseJsonData(GetSection(nalu_packet_header, "packet_header"));
            packet_info_data_location_ = ParseJsonData(GetSection(nalu_packet_header, "packet_info"));
            channel_data_location_ = ParseJsonData(GetSection(nalu_packet_header, "channel"));
            trigger_time_data_location_ = ParseJsonData(GetSection(nalu_packet_header, "trigger_time"));
            logical_position_data_location_ = ParseJsonData(GetSection(nalu_packet_header, "logical_position"));
            window_position_data_location_ = ParseJsonData(GetSection(nalu_packet_header, "window_position"));
        }
    }
};

NaluPacketHeaderParser::~NaluPacketHeaderParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluPacketHeader> 
NaluPacketHeaderParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluPacketHeader>(
                 GetPacketHeader()
                ,GetPacketInfo()
                ,GetChannel()
                ,GetTriggerTime()
                ,GetLogicalPosition()
                ,GetWindowPosition()
            );
}

//Get methods
uint16_t            NaluPacketHeaderParser::GetPacketHeader()           const { return GetData(packet_header_data_location_); }
uint16_t            NaluPacketHeaderParser::GetPacketInfo()             const { return GetData(packet_info_data_location_); }
uint16_t            NaluPacketHeaderParser::GetChannel()                const { return GetData(channel_data_location_); }
uint32_t            NaluPacketHeaderParser::GetTriggerTime()            const { return GetData(trigger_time_data_location_); }
uint16_t            NaluPacketHeaderParser::GetLogicalPosition()        const { return GetData(logical_position_data_location_); }
uint16_t            NaluPacketHeaderParser::GetWindowPosition()         const { return GetData(window_position_data_location_); }

std::ostringstream NaluPacketHeaderParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering NaluPacketHeader: " << std::endl;
    oss << "            PacketHeader: " << "0x" << std::hex << std::setw(4) << std::setfill('0') << GetPacketHeader() << std::endl;
    oss << "            PacketInfo: " << std::dec << GetPacketInfo() << std::endl;
    oss << "            Channel: " << std::dec << GetChannel() << std::endl;
    oss << "            TriggerTime: " << std::dec << GetTriggerTime() << std::endl;
    oss << "            LogicalPosition: " << std::dec << GetLogicalPosition() << std::endl;
    oss << "            WindowPosition: " << std::dec << GetWindowPosition() << std::endl;
    return oss;
}

void NaluPacketHeaderParser::Print() {
    std::cout << this->Stream().str();

}
