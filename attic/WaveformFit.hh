#ifndef WaveformFit_HH 
#define WaveformFit_HH

#include "DataProduct.hh"
#include "Waveform.hh"
#include "TSpline.h"

namespace dataProducts {

    class WaveformFit : public DataProduct {

        public:
            WaveformFit();

            WaveformFit(Waveform* waveform);

            //Destructor
            ~WaveformFit();

            // Crate number
            int crateNum;

            // run/subrun number
            int runNum;
            int subRunNum;

            // AMC slot number
            int amcNum;

            // Channel tag
            int channelTag;

            // Event number
            int eventNum;

            // index of this waveform
            int waveformIndex;

            // Number of samples in the WaveformFit
            unsigned int length;

            // ADC count of pedestal level
            double pedestalLevel;

            // fitter specific
            bool converged;
            double chi2;
            int ndf;
            double nfit; // number of pulses fit 
            std::vector<double> times;
            std::vector<double> amplitudes;

            TRef waveform;
            TRef spline;

            //detector type and number
            std::string detectorType;
            int detectorNum;

            void Show() const override;
            ChannelID GetID() const;

            ClassDefOverride(WaveformFit,3)

    };

    typedef std::vector<WaveformFit> WaveformFitCollection;

}

#endif // WaveformFit_HH