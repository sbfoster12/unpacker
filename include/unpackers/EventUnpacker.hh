#ifndef EVENTRUNPACKER_HH
#define EVENTRUNPACKER_HH

//Standard
#include <iostream>
#include <string>

//Custom
#include "CollectionsHolder.hh"
#include "BankUnpacker.hh"

#define CR_BANK_ID 0

namespace unpackers {

    class EventUnpacker : public CollectionsHolder {
        
    public:
        
        //Constructor
        EventUnpacker();

        //Destructor
        virtual ~EventUnpacker();

        // Virtual function to read the event
        virtual int UnpackEvent(TMEvent* event) = 0;

        virtual int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) = 0;


    private:
        //class name
        const std::string className_;

    protected:
        //bank unpackers
        std::map<int,std::unique_ptr<unpackers::BankUnpacker>> bankUnpackers_;

    };

}

#endif // EVENTRUNPACKER_HH