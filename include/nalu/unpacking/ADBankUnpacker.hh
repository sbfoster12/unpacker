#ifndef ADBANKUNPACKER_HH
#define ADBANKUNPACKER_HH

//Custom
#include "common/unpacking/Logger.hh"
#include "common/unpacking/UnpackerHelpers.hh"
#include "common/unpacking/BankUnpacker.hh"
#include "nalu/unpacking/ADPayloadUnpacker.hh"

#define AD_PAYLOAD_ID 0

namespace unpackers {

    class ADBankUnpacker : public BankUnpacker {
        
    public:
        
        //Constructor
        ADBankUnpacker();

        //Destructor
        ~ADBankUnpacker();

        // Virtual function to unpack the event
        int UnpackBank(TMEvent* event, const std::string& bankName) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;


    private:
        //class name
        const std::string className_;

    protected:
    };

}

#endif // ADBANKUNPACKER_HH