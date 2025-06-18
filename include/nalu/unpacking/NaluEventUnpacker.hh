#ifndef NALUEVENTUNPACKER_HH
#define NALUEVENTUNPACKER_HH

//Custom
#include "common/unpacking/Logger.hh"
#include "common/unpacking/EventUnpacker.hh"
#include "nalu/unpacking/ADBankUnpacker.hh"
#include "nalu/unpacking/ATBankUnpacker.hh"

#define AD_BANK_ID 0
#define AT_BANK_ID 1

namespace unpackers {

    class NaluEventUnpacker : public EventUnpacker {
        
    public:
        
        //Constructor
        NaluEventUnpacker();

        //Destructor
        ~NaluEventUnpacker();

        int UnpackEvent(TMEvent* event) override;

        int UnpackBank(uint64_t* bank_data, unsigned int total_words, int serial_number, std::string bank_name) override;

        /// ROOT class definition macro with versioning
        ClassDefOverride(NaluEventUnpacker,1)

    private:
        //class name
        const std::string className_;
        
    };

}

#endif // NALUEVENTUNPACKER_HH