#ifndef PARSER_HH
#define PARSER_HH

#include <iostream>
#include <string>
#include <vector>

namespace parsers {

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