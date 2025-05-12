#include "BankUnpacker.hh"

using namespace unpackers;

BankUnpacker::BankUnpacker() :
    CollectionsHolder(),
    className_("BankUnpacker")
{}

BankUnpacker::~BankUnpacker() {};

void BankUnpacker::UpdateEventNum(int eventNum) {
    for (auto & unpacker : payloadUnpackers_) {
        unpacker.second->SetEventNum(eventNum);
    }
}

void BankUnpacker::UpdateCrateNum(int crateNum) {
    for (auto & unpacker : payloadUnpackers_) {
        unpacker.second->SetCrateNum(crateNum);
    }
}