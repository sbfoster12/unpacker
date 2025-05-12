#ifndef PileupTagger_HH 
#define PileupTagger_HH

#include "DataProduct.hh"
#include "Waveform.hh"
#include "WaveformIntegral.hh"

namespace dataProducts {

    class PileupTagger : public DataProduct {

        public:
            PileupTagger();

            PileupTagger(WaveformIntegral* w, int window, int before, int after, int dead);

            //Destructor
            ~PileupTagger();

            // run/subrun
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

            void FindPeaks();

            int npeaks;
            std::vector<int> peak_times;
            std::vector<double> peak_amplitudes;
            int averaging_window;
            int samples_before;
            int samples_after;
            int dead_time;


            // ADC count of each sample
            // std::vector<short> trace;

            TRef raw;
            TRef integral;

            void Show() const override;
            void DoIntegration();

            ClassDefOverride(PileupTagger,2)

    };

    typedef std::vector<PileupTagger> PileupTaggerCollection;

}

#endif // PileupTagger_HH