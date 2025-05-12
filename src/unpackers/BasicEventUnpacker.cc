#include "BasicEventUnpacker.hh"

using namespace unpackers;

BasicEventUnpacker::BasicEventUnpacker()
    : EventUnpacker(), 
    className_("BasicEventUnpacker")
{
    utils::LoggerHolder::getInstance()->InfoLogger << "We are constructing the basic event unpacker." << std::endl;

    //Create the bank unpackers
    bankUnpackers_[CR_BANK_ID] = std::make_unique<CRBankUnpacker>();

    //Register the collections in the bank unpacker
    for (const auto& bankUnpacker : bankUnpackers_) {
        this->RegisterCollections(bankUnpacker.second->GetCollections());
    }
}

BasicEventUnpacker::~BasicEventUnpacker() {}

int BasicEventUnpacker::UnpackEvent(TMEvent* event) {

    //Clear previous event
    this->ClearCollections();

    utils::LoggerHolder::getInstance()->InfoLogger << "Unpacking with Event ID: " << event->event_id << " and SN: " << event->serial_number << std::endl;

    // Loop over all banks and read the CR banks
    // Can we parallelize this loop?
    event->FindAllBanks();
    for (const auto& bank : event->banks) {
        if (bank.name.substr(0, 2) == "CR") {
            auto status = bankUnpackers_[CR_BANK_ID]->UnpackBank(event, bank.name);

            if (status == FAILURE_UNPACKING) {
                //Something went wrong, so clear the collections and return failure
                this->ClearCollections();
                return FAILURE_UNPACKING;
            }
        }
    }

    return SUCCESS_UNPACKING;
}

//method to unpack a bank directly instead of the event
int BasicEventUnpacker::UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) {
    if (bankName.substr(0, 2) == "CR") {
        bankUnpackers_[CR_BANK_ID]->ClearCollections();
        int crateNum = std::stoi(bankName.substr(3, 4));
        auto status = bankUnpackers_[CR_BANK_ID]->UnpackBank(bankData, totalWords, serialNumber, crateNum);
        if (status == FAILURE_UNPACKING) {
            //Something went wrong, so clear the collections and return failure
            bankUnpackers_[CR_BANK_ID]->ClearCollections();
            return FAILURE_UNPACKING;
        }
    }
    return SUCCESS_UNPACKING;
}