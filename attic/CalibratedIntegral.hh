#ifndef CalibratedIntegral_HH 
#define CalibratedIntegral_HH

#include "DataProduct.hh"
#include "Waveform.hh"
#include "WaveformIntegral.hh"

namespace dataProducts {

    class CalibratedIntegral : public DataProduct {

        public:
            CalibratedIntegral();

            CalibratedIntegral(WaveformIntegral* w, double e, double t_calib, double t_offset);
            CalibratedIntegral(WaveformIntegral* w, LinearCalibrationHolder map);
            CalibratedIntegral(WaveformIntegral* w, NonLinearCalibrationHolder map);

            //Destructor
            ~CalibratedIntegral();

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

            // Number of samples in the CalibratedIntegral
            unsigned int length;

            // ADC count of pedestal level
            double pedestalLevel;
            double pedestalStdev;

            // energy/timing calibration constants
            double energy_calib;
            double time_calib;
            double time_shift;

            // amplitude of peak relative to pedestal
            double amplitude;

            // number of sigma above the pedestal to start/end the restructed integration window.
            double nsigma;

            // search method
            int search_method;

            // ADC count of each sample
            // std::vector<short> trace;
            double integral;
            double peak_time;
            std::pair<int,int> integration_window;
            double x,y;
            bool is_clipping;
            bool non_linear;

            double fullintegral;

            TRef uncalibrated;
            TRef raw;

            //detector type and number
            std::string detectorType;
            int detectorNum;

            void Show() const override;
            // void DoIntegration();
            void DoNonLinearCalibration(NonLinearCalibrationHolder map);

            ClassDefOverride(CalibratedIntegral,5)

    };

    typedef std::vector<CalibratedIntegral> CalibratedIntegralCollection;

}

#endif // CalibratedIntegral_HH