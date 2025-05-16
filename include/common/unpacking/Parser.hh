#ifndef PARSER_HH
#define PARSER_HH

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>
#include <cstdlib>

//Custom
#include "common/unpacking/Logger.hh"

namespace parsers {

    // Helper methods
    inline uint64_t BitMask(int num_bytes) {
        return (num_bytes >= 8) ? 0xFFFFFFFFFFFFFFFF : ((1ULL << (num_bytes * 8)) - 1);
    }

    struct DataLocation {
        int word_0 = -1;
        int word_1 = -1;
        int bit_0 = -1;
        int bit_1 = -1;
        int num_bytes_0 = -1;
        int num_bytes_1 = -1;
        uint64_t mask_0 = 0;
        uint64_t mask_1 = 0;

        // Constructor
        DataLocation()
            : word_0(-1),
            word_1(-1),
            bit_0(-1),
            bit_1(-1),
            num_bytes_0(-1),
            num_bytes_1(-1),
            mask_0(0),
            mask_1(0)
            {}

        // Constructor
        DataLocation(int word_arg, int bit_arg, int num_bytes_arg)
            : word_0(word_arg),
            word_1(word_arg),
            bit_0(bit_arg),
            bit_1(bit_arg),
            num_bytes_0(num_bytes_arg),
            num_bytes_1(num_bytes_arg),
            mask_0(BitMask(num_bytes_0)),
            mask_1(BitMask(num_bytes_1))
            {}

        // Constructor
        DataLocation(int word_0_arg, int bit_0_arg, int num_bytes_0_arg, int word_1_arg, int bit_1_arg, int num_bytes_1_arg)
            : word_0(word_0_arg),
            word_1(word_1_arg),
            bit_0(bit_0_arg),
            bit_1(bit_1_arg),
            num_bytes_0(num_bytes_0_arg),
            num_bytes_1(num_bytes_1_arg),
            mask_0(BitMask(num_bytes_0)),
            mask_1(BitMask(num_bytes_1))
            {}
    };

    class Parser {
        
    public:
        
        //Constructor
        Parser();

        //Destructor
        virtual ~Parser();

        //Set methods
        void SetWords(std::vector<uint64_t>& words);
        void SetTrailer(std::vector<uint64_t>& words);

        //Get methods
        uint64_t GetSize() const;
        uint64_t GetWord(size_t iWord) const;
        uint64_t GetTrailerWord(size_t iWord) const;

        //Clear
        void Clear();

        // Helper to navigate nested JSON using a reference and structured access
        const nlohmann::json& GetSection(const nlohmann::json& j, const std::string& section_name);

        DataLocation ParseJsonData(const nlohmann::json& data_json);

        uint64_t GetData(const DataLocation& d) const;

        //Print
        virtual std::ostringstream Stream() = 0;
        virtual void Print() = 0;

    private:
        std::string className_ = "Parser";

    protected:
        std::vector<uint64_t> words_;
        std::vector<uint64_t> trailerWords_;
    };
}

#endif // PARSER_HH