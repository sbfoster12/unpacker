#ifndef WAVEFORMHEADER_HH   
#define WAVEFORMHEADER_HH

#include "DataProduct.hh"

namespace dataProducts {

    class WaveformHeader : public DataProduct {

        public:
            WaveformHeader();

            WaveformHeader(unsigned int crateNumArg
                ,unsigned int amcNumArg
                ,unsigned int waveformCountArg
                ,unsigned int waveformGapArg
                ,unsigned int channelTagArg
                ,unsigned int waveformIndexArg
                ,unsigned int fillTypeArg
                ,unsigned int waveformLengthArg
                ,unsigned int DDR3AddressArg
                ,unsigned int preTriggerLength
                ,unsigned char xadcAlarms
                ,bool dummyData
                ,unsigned int triggerWidth
                ,uint64_t timeStamp
            );

            //Destructor
            ~WaveformHeader();

            // associated crate
            unsigned int crateNum;

            // associated slot
            unsigned int amcNum;

            // 12 bit Waveform Count
            unsigned int waveformCount;

            // 22 bit Waveform Gap
            unsigned int waveformGap;

            // 16 bit Channel Tag
            unsigned int channelTag;

            // 12 bit Waveform Index
            unsigned int waveformIndex;

            // 3 bit Fill Type
            unsigned int fillType;

            // 23 bit Waveform Length
            unsigned int waveformLength;

            // 25 bit DDR3 start address
            unsigned int DDR3Address;

            // 12 bit pre trigger length (async mode)
            unsigned int preTriggerLength;

            // 4 bit temp, voltage alaram flags (async mode)
            unsigned char xadcAlarms;

            // dummy or ADC data (async mode)
            bool dummyData;

            // 2 bit front panel input trigger width (async mode)
            unsigned int triggerWidth;

            // 44 bit time stamp (async mode)
            uint64_t timeStamp;

            void Show() const override;

            ClassDefOverride(WaveformHeader,1)

    };

    typedef std::vector<WaveformHeader> WaveformHeaderCollection;

}

#endif // WAVEFORMHEADER_HH