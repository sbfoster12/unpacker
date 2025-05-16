#include "nalu/unpacking/ATPayloadUnpacker.hh"

using namespace unpackers;

ATPayloadUnpacker::ATPayloadUnpacker() 
    : PayloadUnpacker()
    , naluTimePtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , naluTimeParser_(std::make_unique<parsers::NaluTimeParser>())
{
    utils::LoggerHolder::getInstance().InfoLogger << "We are constructing the " << className_ << std::endl;

    //Register the collections
    this->RegisterCollection("NaluTimeCollection",naluTimePtrCol_);

}

ATPayloadUnpacker::~ATPayloadUnpacker() {};

int ATPayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    utils::LoggerHolder::getInstance().InfoLogger << "  We are unpacking an AT payload." << std::endl;

    // Store the starting word for comparison at the end
    unsigned int startWordNum = wordNum;

    // Get the time words
    std::vector<uint64_t> time_words = unpackers::GetXWords(words,wordNum,8,"le");

    // Set the words for the parser
    naluTimeParser_->SetWords(time_words);
    utils::LoggerHolder::getInstance().DebugLogger << naluTimeParser_->Stream().str() << std::endl;

    // Create the data product
    naluTimePtrCol_->push_back(naluTimeParser_->NewDataProduct());

    // Clear data from parser
    naluTimeParser_->Clear();

    return SUCCESS_UNPACKING;
};
