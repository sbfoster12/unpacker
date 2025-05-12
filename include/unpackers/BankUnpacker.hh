#ifndef BANKUNPACKER_HH
#define BANKUNPACKER_HH

//Standard
#include <iostream>
#include <string>
#include <map>

//Custom
#include "midasio.h"
#include "CollectionsHolder.hh"
#include "PayloadUnpacker.hh"

namespace unpackers {

    class BankUnpacker : public CollectionsHolder {
        
    public:
        
        //Constructor
        BankUnpacker();

        //Destructor
        virtual ~BankUnpacker();

        // Virtual function to read the event
        virtual int UnpackBank(TMEvent* event, const std::string& bankName) = 0;

        virtual int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) = 0;

        void UpdateEventNum(int eventNum);
        void UpdateCrateNum(int crateNum);

    private:
        //class name
        const std::string className_;

    protected:
        //unpackers
        std::map<int,std::unique_ptr<unpackers::PayloadUnpacker>> payloadUnpackers_;

    };
}

#endif // BANKUNPACKER_HH