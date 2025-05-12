#ifndef WaveformIntegral_HH 
#define WaveformIntegral_HH

#include "DataProduct.hh"
#include "Waveform.hh"

namespace dataProducts {

    struct SeededIntegralConfig{
        int trigger_low;
        int trigger_high;
    };

    class WaveformIntegral : public DataProduct {

        public:
            WaveformIntegral();

            WaveformIntegral(Waveform* waveform, double nsigma=10.0, int search_method=0, std::tuple<int,int> presample_config={10,250}, int seed_index=-1, int seeded_search_window=-1);

            //Destructor
            ~WaveformIntegral();

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

            // Number of samples in the WaveformIntegral
            unsigned int length;

            // ADC count of pedestal level
            double pedestalLevel;
            double pedestalStdev;

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

            double fullintegral;
            double x,y;
            bool is_clipping;
            bool clipped_integration_window;

            TRef raw;

            //detector type and number
            std::string detectorType;
            int detectorNum;


            ChannelID GetID() const;
            void Show() const override;
            void DoIntegration(std::tuple<int,int> presample_config, int seed_index = -1, int seeded_search_window=-1);
            float GetTime(float time, bool ct_to_ns=false){ return ((Waveform*)raw.GetObject())->GetTime(time, ct_to_ns);};
            std::vector<float> GetTimes(bool ct_to_ns=false){ return ((Waveform*)raw.GetObject())->GetTimes(ct_to_ns);};

            bool is_time_corrected;
            int time_shift;
            int time_scale;
            void CorrectTimes();

            ClassDefOverride(WaveformIntegral,8)

    };

    typedef std::vector<WaveformIntegral> WaveformIntegralCollection;
    typedef std::vector<WaveformIntegral> SeededWaveformIntegralCollection;

}

#endif // WaveformIntegral_HH