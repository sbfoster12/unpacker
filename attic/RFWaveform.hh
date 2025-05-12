#ifndef RFWaveform_HH 
#define RFWaveform_HH

#include "DataProduct.hh"
#include "Waveform.hh"
#include "WaveformIntegral.hh"
#include "TF1.h"
#include "TMatrixD.h"
#include "TVectorD.h"

namespace dataProducts {

    struct RFConfig{
        double frequency;
        int amcNum;
        int channelTag;
        int nsamples;
        bool do_rf;
    };

    class RFWaveform : public DataProduct {

        public:
            RFWaveform();

            RFWaveform(Waveform* waveform, RFConfig config);

            //Destructor
            ~RFWaveform();

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
            int waveformIndex;

            // ADC count of pedestal level
            double pedestalLevel;
            double pedestalStdev;
            bool is_clipping;

            double frequency;
            double phase;
            double amplitude;
            double constant;

            double corrected_phase;
            double t0_in_trigger_window;

            TRef raw;

            ChannelID GetID();
            void Show() const override;
            void IdentifyPhase(RFConfig config);
            void FitSine(std::vector<short> *trace, std::vector<float> *wfTime, double &ampl, double &phi, double &constant, int nsamples);
            void CorrectPhaseT0(WaveformIntegral *t0, int trigger_window_low, int trigger_window_high);

            ClassDefOverride(RFWaveform, 2)

    };

    typedef std::vector<RFWaveform> RFWaveformCollection;

}

#endif // RFWaveform_HH