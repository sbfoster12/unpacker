#include "CRBankUnpacker.hh"

using namespace unpackers;

CRBankUnpacker::CRBankUnpacker() :
    BankUnpacker(),
    className_("CRBankUnpacker")
{
    utils::LoggerHolder::getInstance()->InfoLogger << "We are constructing the CR bank unpacker." << std::endl;

    //Initialize the unpackers
    payloadUnpackers_[WFD5_BOARD_TYPE] = std::make_unique<unpackers::WFD5Unpacker>();
    payloadUnpackers_[FC7_BOARD_TYPE] = std::make_unique<unpackers::FC7Unpacker>();

    //Register the collections in the payload unpackers
    for (const auto& payloadUnpacker : payloadUnpackers_) {
        this->RegisterCollections(payloadUnpacker.second->GetCollections());
    }
}

CRBankUnpacker::~CRBankUnpacker() {};

int CRBankUnpacker::UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) {

    //Update the unpackers event and crate numbers
    this->UpdateEventNum(serialNumber);
    this->UpdateCrateNum(crateNum);

    //keep track of which word we are parsing
    unsigned int wordNum = 0;

    //Loop over the payloads
    while (wordNum < totalWords) {
        //Determine what payload we are looking at
        unsigned int tmpNum = wordNum;
        auto tmpwords = unpackers::GetXWords(bankData,tmpNum,2,"bigendian");
        int payload_type = unpackers::ExtractBits(tmpwords[1],13,15);
        //int data_length = unpackers::ExtractBits(tmpwords[0],0,19);

        //Get the unpacker and unpack this payload
        if (payloadUnpackers_.find(payload_type) != payloadUnpackers_.end()) {
            auto status = payloadUnpackers_[payload_type]->Unpack(bankData,wordNum);

            if (status == FAILURE_UNPACKING) {
                //Something went wrong, so clear the collections and return failure
                this->ClearCollections();
                return FAILURE_UNPACKING;
            }

        } else {
            std::cerr << "Error: Payload unpacker with given type was not found.\n"
            << "Location: CRBankUnpacker::UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum)\n"
            << "Details: the provided payload id was " << payload_type << std::endl;
            return FAILURE_UNPACKING;
        }
    }

    return SUCCESS_UNPACKING;
}

int CRBankUnpacker::UnpackBank(TMEvent* event, const std::string& bankName) {

    //Get the bank
    TMBank *bank = event->FindBank(bankName.c_str());

    //Update the unpackers event and crate numbers
    this->UpdateEventNum(event->serial_number);
    this->UpdateCrateNum(std::stoi(bank->name.substr(3, 4)));

    //Get total number of words (each word is 8 bytes) 
    unsigned int totalWords = bank->data_size/8;

    if (bank != NULL) {
        uint64_t* bankData = reinterpret_cast<uint64_t*>(event->GetBankData(bank));
        return this->UnpackBank(bankData, totalWords, event->serial_number, std::stoi(bank->name.substr(3, 4)));
    } else {
        utils::LoggerHolder::getInstance()->InfoLogger <<"  No CR bank in event ID: " <<  event->event_id << " SN: " << event->serial_number << std::endl;
        return FAILURE_UNPACKING;
    }
}