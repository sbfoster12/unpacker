#include "FC7Unpacker.hh"

using namespace unpackers;

FC7Unpacker::FC7Unpacker() 
    : PayloadUnpacker()
    , FC7HeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , FC7HeaderParser_(std::make_unique<parsers::FC7HeaderParser>())
{
    utils::LoggerHolder::getInstance()->InfoLogger << "We are constructing the FC7 unpacker." << std::endl;

    //Fill the collections
    this->RegisterCollection("FC7HeaderCollection",FC7HeaderPtrCol_);

};

FC7Unpacker::~FC7Unpacker() {};

int FC7Unpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    utils::LoggerHolder::getInstance()->InfoLogger << "\tWe are unpacking a FC7 payload." << std::endl;

    /*
        First up is the AMC13+FC7 header, which is 3 words long
    */

    auto header_words = GetXWords(words,wordNum,3,"bigendian");

    //Set the words in the parser
    FC7HeaderParser_->SetWords(header_words);

    //Get the data length
    int data_length = FC7HeaderParser_->DataLength();

    //Parse and create the data product
    FC7HeaderPtrCol_->push_back(FC7HeaderParser_->NewDataProduct(crateNum_));

    //Clear the parser
    utils::LoggerHolder::getInstance()->DebugLogger << FC7HeaderParser_->Stream().str();
    FC7HeaderParser_->Clear();

    /*
        Do more parsing
    */

    //Increment wordNum
    wordNum-=3; // for AMC13+FC7 header
    wordNum+=data_length;

    return SUCCESS_UNPACKING;
};