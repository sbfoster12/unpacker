#ifndef WFD5HEADER_HH   
#define WFD5HEADER_HH

#include "DataProduct.hh"

namespace dataProducts {

    class WFD5Header : public DataProduct {

        public:
            WFD5Header();

            WFD5Header(
                unsigned int crateNumArg
                ,unsigned int amcNum
                ,unsigned int triggerNum
                ,unsigned long clockCounter
                ,unsigned int fillType
                ,unsigned int boardType
                ,unsigned int boardID
                ,unsigned char xadcAlarms
                ,bool emptyEvent
                ,unsigned int majorRevision
                ,unsigned int minorRevision
                ,unsigned int patchRevision
            );

            //Destructor
            ~WFD5Header();

            //member variables

            // crate number
            unsigned int crateNum;

            // uTCA slot number (AMCNo)
            unsigned int amcNum;

            // trigger number of this rider
            unsigned int triggerNum;

            // clock counter of this rider ([BXid, OrN])
            unsigned long clockCounter;

            // fill type of this event
            unsigned int fillType;

            // board type of this amc (rider, ccc fc7, tracker fc7)
            unsigned int boardType;

            // board ID
            unsigned int boardID;

            unsigned char xadcAlarms;

            bool emptyEvent;

            // revision numbers
            unsigned int majorRevision;
            unsigned int minorRevision;
            unsigned int patchRevision;

            void Show() const override;

            ClassDefOverride(WFD5Header,1)

    };

    typedef std::vector<WFD5Header> WFD5HeaderCollection;
}

#endif // WFD5HEADER_HH