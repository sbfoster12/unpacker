#include "ODB.hh"
#include <numeric>

using namespace dataProducts;

ODB::ODB()
    : DataProduct()
    ,odbString("")
{}

ODB::ODB(std::string odbStringArg)
    : DataProduct()
    ,odbString(odbStringArg)
    ,runNumber(0)
    ,subRunNumber(0)
    ,xStage(0)
    ,yStage(0)
{
    ParseODB();
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void ODB::ParseODB() {

    boost::property_tree::ptree odbTree;

    std::stringstream xmlstream(odbString);

    boost::property_tree::read_xml(xmlstream, odbTree);
    odbTree = odbTree.get_child("odb");

    ODBParser odbParser(odbString);

    //run number
    const auto& runInfoDir =  *(odbParser.GetDirectory({"Runinfo"}));
    std::string runNumString = odbParser.GetValue<std::string>(runInfoDir, {"Run number"});
    runNumber = std::atoi(runNumString.c_str());
    std::cout << "     -> parsed Run string from the ODB: " << runNumString << std::endl;
    std::cout << "     -> Run from the ODB: " << runNumber << std::endl;

    //subRun number
    const auto& loggerSettingsDir =  *(odbParser.GetDirectory({"Logger","Channels","0","Settings"}));
    std::string file_name = odbParser.GetValue<std::string>(loggerSettingsDir, {"Current filename"});
    try {
        size_t start1 = file_name.find("run") + 3;
        size_t end1 = file_name.find("_", start1);
        std::string num1 = file_name.substr(start1, end1 - start1);

        // Find the start and end of the second number
        size_t start2 = end1 + 1;
        size_t end2 = file_name.find(".", start2);
        std::string num2 = file_name.substr(start2, end2 - start2);

        // Convert strings to integers
        std::stringstream(num2) >> subRunNumber;
        std::cout << "     -> parsed Subrun string from the ODB: " << num2 << std::endl;
        std::cout << "     -> Subrun from the ODB: " << subRunNumber << std::endl;
    }
    catch (...)
    {
        std::cerr << "Warning: Unable to subrun from ODB" << std::endl;
    }

    //Digitization frequencies
    const auto& amc1300SettingsDir =  *(odbParser.GetDirectory({"Equipment", "AMC1300", "Settings"}));
    for (const auto& riderdirp : odbParser.GetSubDirectories(amc1300SettingsDir, "Rider", false)) {
        auto wfdName = odbParser.GetDirectoryName(*riderdirp);
        auto wfdNum = atoi(wfdName.substr(5).c_str());
        const auto& wfdDir =  *(odbParser.GetDirectory({"Equipment", "AMC1300", "Settings", wfdName}));
        std::string freq = odbParser.GetValue<std::string>(wfdDir, {"Board","Digitization Frequency (MHz)"});
        digitizationFrequencies[wfdNum] =std::atoi(freq.c_str());

        std::string presamples = odbParser.GetValue<std::string>(wfdDir, {"Board","Async CBuf Waveform Presamples"});
        waveformPresamples[wfdNum] =std::atof(freq.c_str());
    }

    //Stage position
    const auto& ISELDir =  odbParser.GetDirectory({"Equipment", "ISEL", "Variables"});
    if ( ISELDir != nullptr) {
        std::vector<int> stage = odbParser.GetVector<int>(*ISELDir, {"Output"});
        xStage = stage[0];
        yStage = stage[1];
    } else {
        std::cout << "Didn't find Equipment/ISEL/Variables/Ouput" << std::endl;
    }

    //Want to get more from the ODB? Add it here and make a member variable!

}

ODB::~ODB() {}


void ODB::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "ODB:" << std::endl;
    oss << runNumber << std::endl;
    oss << subRunNumber << std::endl;
    for (const auto & pair : digitizationFrequencies) {
            oss << "WFD " << pair.first << " digitization frequency = " << pair.second << std::endl;
    }
    oss << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}