#ifndef CRBANKUNPACKER_HH
#define CRBANKUNPACKER_HH

//Custom
#include "Logger.hh"
#include "BankUnpacker.hh"
#include "WFD5Unpacker.hh"
#include "FC7Unpacker.hh"

#define WFD5_BOARD_TYPE 1
#define FC7_BOARD_TYPE 2

namespace unpackers {

    class CRBankUnpacker : public BankUnpacker {
        
    public:
        
        //Constructor
        CRBankUnpacker();

        //Destructor
        ~CRBankUnpacker();

        // Virtual function to unpack the event
        int UnpackBank(TMEvent* event, const std::string& bankName) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;


    private:
        //class name
        const std::string className_;

    protected:
    };

}

#endif // CRBANKUNPACKER_HH