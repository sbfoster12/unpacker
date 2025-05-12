#include "PayloadUnpacker.hh"

using namespace unpackers;

PayloadUnpacker::PayloadUnpacker() 
    : CollectionsHolder(),
    crateNum_(0),
    eventNum_(0)
{};

PayloadUnpacker::~PayloadUnpacker() {};

void PayloadUnpacker::SetCrateNum(int crateNum) { crateNum_ = crateNum; }
void PayloadUnpacker::SetEventNum(int eventNum) { eventNum_ = eventNum; }