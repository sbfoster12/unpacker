#include "nalu/unpacking/NaluEventUnpacker.hh"

using namespace unpackers;

NaluEventUnpacker::NaluEventUnpacker()
    : EventUnpacker(), 
    className_("NaluEventUnpacker")
{
    utils::LoggerHolder::getInstance().InfoLogger << "We are constructing the " << className_ << std::endl;

    bankUnpackers_[AD_BANK_ID] = this->MakeAndRegister<ADBankUnpacker>();
    bankUnpackers_[AT_BANK_ID] = this->MakeAndRegister<ATBankUnpacker>();

}

NaluEventUnpacker::~NaluEventUnpacker() {}

int NaluEventUnpacker::UnpackEvent(TMEvent* event) {

    //Clear previous event
    this->ClearCollections();

    utils::LoggerHolder::getInstance().InfoLogger << std::dec << "Unpacking Event ID: " << event->event_id << ", SN: " << event->serial_number << std::endl;

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

    return UNPACKING_SUCCESS;
}

//method to unpack a bank directly instead of the event
int NaluEventUnpacker::UnpackBank(uint64_t* bank_data, unsigned int total_words, int serial_number, std::string bank_name) {

    // Figure out which bank ID it is
    int bank_ID = 0;
    if (bank_name.substr(0, 2) == "AD") {
        bank_ID = AD_BANK_ID;
    } else if (bank_name.substr(0, 2) == "AT") {
        bank_ID = AT_BANK_ID;
    }

    // Check it is a valid ID
    if (bankUnpackers_.find(bank_ID) != bankUnpackers_.end()) {

        // Get the bank unpacker
        bankUnpackers_[AD_BANK_ID]->ClearCollections();
        int bank_num = std::stoi(bank_name.substr(3, 4));
        auto status = bankUnpackers_[AD_BANK_ID]->UnpackBank(bank_data, total_words, serial_number, bank_num);
        if (status == UNPACKING_FAILURE) {
            //Something went wrong, so clear the collections and return failure
            bankUnpackers_[AD_BANK_ID]->ClearCollections();
            return UNPACKING_FAILURE;
        }
    }

    return UNPACKING_SUCCESS;
}