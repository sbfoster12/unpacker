#ifndef BASICEVENTUNPACKER_HH
#define BASICEVENTUNPACKER_HH

//Custom
#include "Logger.hh"
#include "CRBankUnpacker.hh"
#include "EventUnpacker.hh"

namespace unpackers {

    class BasicEventUnpacker : public EventUnpacker {
        
    public:
        
        //Constructor
        BasicEventUnpacker();

        //Destructor
        ~BasicEventUnpacker();

        int UnpackEvent(TMEvent* event) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) override;


    private:
        //class name
        const std::string className_;
        
    };

}

#endif // BASICEVENTUNPACKER_HH