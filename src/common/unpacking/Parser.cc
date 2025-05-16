#include "common/unpacking/Parser.hh"

using namespace parsers;

Parser::Parser()
    : words_({})
    ,trailerWords_({})
{}

Parser::~Parser() {}

void Parser::SetWords(std::vector<uint64_t>& words) { words_=std::move(words); }
void Parser::SetTrailer(std::vector<uint64_t>& words) { trailerWords_ = std::move(words); }

uint64_t Parser::GetSize() const { 
    uint64_t bankSize = words_.size();
    return bankSize;
}

uint64_t Parser::GetWord(size_t iWord) const {
    if (words_.size() == 0) {
        throw "words is empty!";
    }
    if (iWord > words_.size()  - 1) {
        throw "words is shorter than requested length";
    }
    return words_.at(iWord);
}

uint64_t Parser::GetTrailerWord(size_t iWord) const {
    if (trailerWords_.size() == 0) {
        throw "words is empty!";
    }
    if (iWord > trailerWords_.size() - 1) {
        throw "words is shorter than requested length";
    }
    return trailerWords_.at(iWord);
}

void Parser::Clear() { words_.clear(); trailerWords_.clear(); }

// Helper to navigate nested JSON using a reference and structured access
const nlohmann::json& Parser::GetSection(const nlohmann::json& j, const std::string& section_name) {
    if (!j.contains(section_name)) {
        throw std::runtime_error("'" + section_name + "' section is missing");
    }
    return j.at(section_name);
}

//Helper method to put json data into DataLocation struct
DataLocation Parser::ParseJsonData(const nlohmann::json& data_json) {
    DataLocation data;

    // Either single-word or split-word logic
    if (data_json.contains("word")) {
        data.word_0 = data_json["word"].get<int>();
        data.word_1 = data.word_0;
    } else if (data_json.contains("word_0") && data_json.contains("word_1")) {
        data.word_0 = data_json["word_0"].get<int>();
        data.word_1 = data_json["word_1"].get<int>();
    } else {
        throw std::runtime_error("Missing 'word' or 'word_0'/'word_1' in data");
    }

    // Same for bit positions
    if (data_json.contains("bit")) {
        data.bit_0 = data_json["bit"].get<int>();
        data.bit_1 = data.bit_0;
    } else if (data_json.contains("bit_0") && data_json.contains("bit_1")) {
        data.bit_0 = data_json["bit_0"].get<int>();
        data.bit_1 = data_json["bit_1"].get<int>();
    } else {
        throw std::runtime_error("Missing 'bit' or 'bit_0'/'bit_1' in data");
    }

    // Same for num_bytes positions
    if (data_json.contains("num_bytes")) {
        data.num_bytes_0 = data_json["num_bytes"].get<int>();
        data.num_bytes_1 = data.num_bytes_0;
        data.mask_0 = BitMask(data.num_bytes_0);
        data.mask_1 = data.mask_0;
    } else if (data_json.contains("num_bytes_0") && data_json.contains("num_bytes_1")) {
        data.num_bytes_0 = data_json["num_bytes_0"].get<int>();
        data.num_bytes_1 = data_json["num_bytes_1"].get<int>();
        data.mask_0 = BitMask(data.num_bytes_0);
        data.mask_1 = BitMask(data.num_bytes_1);
    } else {
        throw std::runtime_error("Missing 'num_bytes' or 'num_bytes_0'/'num_bytes_1' in data");
    }

    return data;
}

// Get the data from the words using the configuration define in a DataLocation struct
uint64_t Parser::GetData(const DataLocation& d) const {
    if (d.word_0 == d.word_1) {
        // Single word
        return (GetWord(d.word_0) >> d.bit_0) & d.mask_0;
    } else {
        // Multi-word
        uint64_t part0 = (GetWord(d.word_0) >> d.bit_0) & d.mask_0;
        uint64_t part1 = (GetWord(d.word_1) >> d.bit_1) & d.mask_1;
        return part0 | (part1 << 8*d.num_bytes_0); 
    }
}
