#ifndef WFD5HEADERPARSER_HH
#define WFD5HEADERPARSER_HH

//Custom
#include "Parser.hh"
#include "WFD5Header.hh"

// Bit locations and masks for AMCPayload format:
#define AMCNo_WORD 0
#define AMCNo_BIT 56
#define AMCNo_MASK 0xf
#define LV1ID_WORD 0
#define LV1ID_BIT 32
#define LV1ID_MASK 0xffffff
#define BcN_WORD 0
#define BcN_BIT 20
#define BcN_MASK 0xfff
#define OrN_WORD 1
#define OrN_BIT 16
#define OrN_MASK 0xffffffff
#define DataLength_WORD 0
#define DataLength_BIT 0
#define DataLength_MASK 0xfffff
#define TriggerType_WORD 1
#define TriggerType_BIT 48
#define TriggerType_MASK 0x1f
#define Endianness_WORD 1
#define Endianness_BIT 53
#define Endianness_MASK 0x1
#define BoardID_WORD 1
#define BoardID_BIT 0
#define BoardID_MASK 0xfff
#define BoardType_WORD 1
#define BoardType_BIT 13
#define BoardType_MASK 0x7
#define ASyncMode_WORD 1
#define ASyncMode_BIT 12
#define ASyncMode_MASK 0x1
#define CRC_BIT 32
#define CRC_MASK 0xffffffff
#define LV1ID_TR_BIT 24
#define LV1ID_TR_MASK 0xff
#define DataLength_TR_BIT 0
#define DataLength_TR_MASK 0xfffff
#define XADCALARM_WORD 1
#define XADCALARM_BIT 54
#define XADCALARM_MASK 0xf
#define EMPTYEVENT_WORD 1
#define EMPTYEVENT_BIT 53
#define EMPTYEVENT_MASK 0x1
#define MajorRevision_WORD 2
#define MajorRevision_BIT 16
#define MajorRevision_MASK 0xff
#define MinorRevision_WORD 2
#define MinorRevision_BIT 8
#define MinorRevision_MASK 0xff
#define PatchRevision_WORD 2
#define PatchRevision_BIT 0
#define PatchRevision_MASK 0xff
#define ChannelsEnabled_WORD 1
#define ChannelsEnabled_BIT 59
#define ChannelsEnabled_MASK 0x1f

namespace parsers {

    class WFD5HeaderParser : public Parser {
        
    public:
        
        //Constructor
        WFD5HeaderParser();

        //Destructor
        ~WFD5HeaderParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::WFD5Header> NewDataProduct(unsigned int crateNum);

        //Get methods
        uint32_t        AMCNo() const;
        uint32_t        LV1ID() const;
        uint32_t        BcN() const;
        uint32_t        OrN() const;
        uint64_t        ClockCounter() const;
        uint32_t        DataLength() const;
        uint32_t        TriggerType() const;
        uint32_t        Endianness() const;
        uint32_t        BoardID() const;
        uint32_t        BoardType() const;
        uint32_t        ASyncMode() const;
        uint32_t        CRC() const;
        uint32_t        LV1ID_TR() const;
        uint32_t        DataLength_TR() const;
        unsigned char   XADCAlarm() const;
        bool            EmptyEvent() const;
        unsigned int    MajorRevision() const;
        unsigned int    MinorRevision() const;
        unsigned int    PatchRevision() const;
        unsigned int    NumEnabledChannels() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "WFD5HeaderParser";

    };
}

#endif // WFD5HEADERPARSER_HH