#include "nalu/unpacking/ATBankUnpacker.hh"

using namespace unpackers;

ATBankUnpacker::ATBankUnpacker() :
    BankUnpacker(),
    className_("ATBankUnpacker")
{
    utils::LoggerHolder::getInstance()->InfoLogger << "We are constructing the " << className_ << std::endl;

    payloadUnpackers_[AT_PAYLOAD_ID] = this->MakeAndRegister<unpackers::ATPayloadUnpacker>();

}

ATBankUnpacker::~ATBankUnpacker() {};

int ATBankUnpacker::UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) {

    // Update the unpackers event and crate numbers
    this->UpdateEventNum(serialNumber);
    this->UpdateCrateNum(crateNum);

    // Keep track of which word we are parsing
    unsigned int wordNum = 0;

    auto status = payloadUnpackers_[AT_PAYLOAD_ID]->Unpack(bankData,wordNum);

    if (status == UNPACKING_FAILURE) {
        // Something went wrong, so clear the collections and return failure
        this->ClearCollections();
        return UNPACKING_FAILURE;
    }

    return UNPACKING_SUCCESS;
}

int ATBankUnpacker::UnpackBank(TMEvent* event, const std::string& bankName) {

    //Get the bank
    TMBank *bank = event->FindBank(bankName.c_str());

    //Update the unpackers event and crate numbers
    this->UpdateEventNum(event->serial_number);
    this->UpdateCrateNum(std::stoi(bank->name.substr(3, 4)));

    //Get total number of words (each word is 8 bytes) 
    unsigned int totalWords = bank->data_size/8;
    // std::cout << std::dec << "totalWords " << totalWords << std::endl;

    if (bank != NULL) {
        uint64_t* bankData = reinterpret_cast<uint64_t*>(event->GetBankData(bank));
        return this->UnpackBank(bankData, totalWords, event->serial_number, std::stoi(bank->name.substr(3, 4)));
    } else {
        utils::LoggerHolder::getInstance()->InfoLogger <<"  No AT bank in event ID: " <<  event->event_id << " SN: " << event->serial_number << std::endl;
        return UNPACKING_FAILURE;
    }
}