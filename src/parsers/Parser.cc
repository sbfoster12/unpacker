#include "Parser.hh"

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