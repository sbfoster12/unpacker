#ifndef WAVEFORM_HH 
#define WAVEFORM_HH

#include "DataProduct.hh"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLatex.h"
// #include "ODB.hh"


namespace dataProducts {

    class Waveform : public DataProduct {

        public:
            Waveform();

            Waveform(int crateNum
                ,int amcNum
                ,int channelTag
                ,int eventNum
                ,int waveformIndex
                ,unsigned int length
                ,double pedestalLevel
                ,double pedestalStdev
                ,int firstSampleNum
                ,unsigned long clockCounter
                ,int preTriggerLength 
                ,int runNum
                ,int subRunNum
                ,std::vector<short> trace
            );

            Waveform(Waveform* w);

            //Destructor
            ~Waveform();

            // run/subrun number
            int runNum;
            int subRunNum;

            // Crate number
            int crateNum;

            // AMC slot number
            int amcNum;

            // Channel tag
            int channelTag;

            // Event number
            int eventNum;

            // index of this waveform
            int waveformIndex;

            // Number of samples in the waveform
            unsigned int length;

            // ADC count of pedestal level
            double pedestalLevel;
            double pedestalStdev;

            double x,y;
            bool is_clipping;

            // first sample number of the trace
            int firstSampleNum;

            // 40 MHz TTC trigger timestamp
            unsigned long clockCounter;

            // pre trigger length
            int preTriggerLength;
            double digitizationFrequency;
            int digitizationShift; //number of 40 MHz clock cycles to shift this by, relative to the T0 trigger

            // ADC count of each sample
            std::vector<short> trace;

            //detector type and number
            std::string detectorType;
            int detectorNum;
            
            // visualization utilities
            void Show() const override;
            
            TRef raw;
            // TRef odb;

            ClassDefOverride(Waveform,1)

    };

    typedef std::vector<Waveform> WaveformCollection;
}

#endif // WAVEFORM_HH