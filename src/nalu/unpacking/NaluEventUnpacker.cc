#include "nalu/unpacking/NaluEventUnpacker.hh"

using namespace unpackers;

NaluEventUnpacker::NaluEventUnpacker()
    : EventUnpacker(), 
    className_("NaluEventUnpacker")
{
    utils::LoggerHolder::getInstance()->InfoLogger << "We are constructing the " << className_ << std::endl;

    bankUnpackers_[AD_BANK_ID] = this->MakeAndRegister<ADBankUnpacker>();
    bankUnpackers_[AT_BANK_ID] = this->MakeAndRegister<ATBankUnpacker>();

}

NaluEventUnpacker::~NaluEventUnpacker() {}

int NaluEventUnpacker::UnpackEvent(TMEvent* event) {

    //Clear previous event
    this->ClearCollections();

    utils::LoggerHolder::getInstance()->InfoLogger << std::dec << "Unpacking Event ID: " << event->event_id << ", SN: " << event->serial_number << std::endl;

    // Loop over all banks and read the AD banks
    event->FindAllBanks();
    for (const auto& bank : event->banks) {
        if (bank.name.substr(0, 2) == "AD") {
            auto status = bankUnpackers_[AD_BANK_ID]->UnpackBank(event, bank.name);

            if (status == UNPACKING_FAILURE) {
                //Something went wrong, so clear the collections and return failure
                this->ClearCollections();
                return UNPACKING_FAILURE;
            }
        } else if (bank.name.substr(0, 2) == "AT") {
            auto status = bankUnpackers_[AT_BANK_ID]->UnpackBank(event, bank.name);

            if (status == UNPACKING_FAILURE) {
                //Something went wrong, so clear the collections and return failure
                this->ClearCollections();
                return UNPACKING_FAILURE;
            }
        }
    }

    return SUCCESS_UNPACKING;
}

//method to unpack a bank directly instead of the event
int NaluEventUnpacker::UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) {
    // if (bankName.substr(0, 2) == "CR") {
    //     bankUnpackers_[CR_BANK_ID]->ClearCollections();
    //     int crateNum = std::stoi(bankName.substr(3, 4));
    //     auto status = bankUnpackers_[CR_BANK_ID]->UnpackBank(bankData, totalWords, serialNumber, crateNum);
    //     if (status == FAILURE_UNPACKING) {
    //         //Something went wrong, so clear the collections and return failure
    //         bankUnpackers_[CR_BANK_ID]->ClearCollections();
    //         return FAILURE_UNPACKING;
    //     }
    // }
    return SUCCESS_UNPACKING;
}