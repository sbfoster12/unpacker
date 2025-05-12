#ifndef IntegralCluster_HH 
#define IntegralCluster_HH

#include "DataProduct.hh"
#include "Waveform.hh"
#include "WaveformIntegral.hh"
#include "CalibratedIntegral.hh"

namespace dataProducts {

    struct ClusterConfig{
        int position_weighting_mode;
        double log_w;
    };

    class IntegralCluster : public DataProduct {

        public:
            IntegralCluster();

            template<typename T> IntegralCluster(T *w, ClusterConfig config);
            // IntegralCluster(WaveformIntegralCollection* w);
            // IntegralCluster(CalibratedIntegralCollection* w);

            //Destructor
            ~IntegralCluster();

            // run/subrun
            int runNum;
            int subRunNum;

            // Event number
            int eventNum;

            // index of this waveform
            int waveformIndex;

            // properties to be summed
            double amplitude;
            double integral;
            double fullintegral;

            // properties to be calculated
            double x;
            double y;
            double geom_mean;
            double geom_mean_secondaries;
            // int max_amplitude_channel;
            // int max_amplitude_amc;
            // int max_amplitude_crate;
            // double max_integral;  

            // properties from all xtals
            bool is_clipping;
            TRef max_channel;          
            TRefArray xtals;

            void Show() const override;
            // void DoClustering(ClusterConfig config);
            template<typename T> void DoClustering(T *w, ClusterConfig config);

            ClassDefOverride(IntegralCluster,3)

    };

    typedef std::vector<IntegralCluster> IntegralClusterCollection;

}

#endif // IntegralCluster_HH