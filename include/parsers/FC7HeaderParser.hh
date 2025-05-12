#ifndef FC7HEADERPARSER_HH
#define FC7HEADERPARSER_HH

#include "Parser.hh"
#include "FC7Header.hh"

// Bit locations and masks for AMCPayload format:
#define WFD5_BT 1
#define FC7_BT 2

#define FC7_AMCNo_WORD 0
#define FC7_AMCNo_BIT 56
#define FC7_AMCNo_MASK 0xf
#define FC7_TrigNum_WORD 0
#define FC7_TrigNum_BIT 32
#define FC7_TrigNum_MASK 0xffffff
#define FC7_BcN_WORD 0
#define FC7_BcN_BIT 20
#define FC7_BcN_MASK 0xfff
#define FC7_OrN_WORD 1
#define FC7_OrN_BIT 16
#define FC7_OrN_MASK 0xffffffff
#define FC7_DataLength_WORD 0
#define FC7_DataLength_BIT 0
#define FC7_DataLength_MASK 0xfffff
#define FC7_TriggerType_WORD 1
#define FC7_TriggerType_BIT 48
#define FC7_TriggerType_MASK 0x1f
#define FC7_BoardID_WORD 1
#define FC7_BoardID_BIT 0
#define FC7_BoardID_MASK 0x7ff
#define FC7_BoardType_WORD 1
#define FC7_BoardType_BIT 13
#define FC7_BoardType_MASK 0x7
#define FC7_FC7Type_WORD 1
#define FC7_FC7Type_BIT 11
#define FC7_FC7Type_MASK 0x3
#define FC7_CRC_BIT 32
#define FC7_CRC_MASK 0xffffffff
#define FC7_TrigNum_TR_BIT 24
#define FC7_TrigNum_TR_MASK 0xff
#define FC7_DataLength_TR_BIT 0
#define FC7_DataLength_TR_MASK 0xfffff
#define FC7_XADCALARM_WORD 1
#define FC7_XADCALARM_BIT 54
#define FC7_XADCALARM_MASK 0xf
#define FC7_EMPTYEVENT_WORD 1
#define FC7_EMPTYEVENT_BIT 53
#define FC7_EMPTYEVENT_MASK 0x1
#define FC7_MajorRevision_WORD 2
#define FC7_MajorRevision_BIT 16
#define FC7_MajorRevision_MASK 0xff
#define FC7_MinorRevision_WORD 2
#define FC7_MinorRevision_BIT 8
#define FC7_MinorRevision_MASK 0xff
#define FC7_PatchRevision_WORD 2
#define FC7_PatchRevision_BIT 0
#define FC7_PatchRevision_MASK 0xff

namespace parsers {

    class FC7HeaderParser : public Parser {
        
    public:
        
        //Constructor
        FC7HeaderParser();

        //Destructor
        ~FC7HeaderParser();

        //Method to make the data product
        std::unique_ptr<dataProducts::FC7Header> NewDataProduct(unsigned int crateNum);

        //Get methods
        uint32_t            AMCNo() const;
        uint32_t            TrigNum() const;
        uint32_t            BcN() const;
        uint32_t            OrN() const;
        uint64_t            TimeStamp() const;
        uint32_t            DataLength() const;
        uint32_t            TriggerType() const;
        uint32_t            BoardID() const;
        uint32_t            BoardType() const;
        uint32_t            FC7Type() const;
        uint32_t            CRC() const;
        uint32_t            TrigNum_TR() const;
        uint32_t            DataLength_TR() const;
        unsigned char       XADCAlarm() const;
        bool                EmptyEvent() const;
        unsigned int        MajorRevision() const;
        unsigned int        MinorRevision() const;
        unsigned int        PatchRevision() const;


        //Print method
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "FC7HeaderParser";

    };
}

#endif // FC7HEADERPARSER_HH