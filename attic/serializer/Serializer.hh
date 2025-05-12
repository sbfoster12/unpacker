#ifndef SERIALIZER_HH
#define SERIALIZER_HH

//Standard
#include <iostream>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <nlohmann/json.hpp>

//Custom
#include "dataProducts/Waveform.hh"
#include "dataProducts/WaveformFitter.hh"
#include "dataProducts/WaveformIntegral.hh"
#include "dataProducts/IntegralCluster.hh"
#include "dataProducts/HistogramMaker.hh"
#include "dataProducts/RFWaveform.hh"
#include "dataProducts/ChannelConfig.hh"

namespace unpackers {

    class Serializer {
        
    public:
        
        //Constructors
        Serializer();
        Serializer(std::string inputFile, int run, int subRun, int event);

        //Destructor
        ~Serializer();

        //Method to load the channel mapping
        void LoadChannelMapping();

        void SetRun(int run);
        void SetSubRun(int subRun);
        void SetEvent(int event);
        void SetWaveforms(const dataProducts::WaveformCollection& inputWaveforms);

        double GetJitterCorrection(dataProducts::Waveform* w);
        double GetNSigmaIntegration(dataProducts::Waveform* w);
        std::tuple<int,int> GetPresamplesWindowSize(dataProducts::Waveform* w);
        double GetIntegralSizeCutoff(dataProducts::Waveform* w);
        std::string GetSplineFile() { return spline_file_; };
        bool DoTemplateFit(){ return do_template_fit_; };
        void FitterSetup( dataProducts::WaveformFitter* w );
        std::string GetString() const;
        dataProducts::WaveformFitterConfig GetFitterConfig() {return fitter_config;};

        // these could be called by anything which has the members crateNum, amcNum, channelTag
        // template <typename T> double GetEnergyCalibration(T *waveform);
        template <typename T> double GetTimingCalibration(T *waveform);
        template <typename T> double GetTimingShift(T *waveform);
        template <typename T> bool IsNonLinear(T *waveform);
        template <typename T> dataProducts::LinearCalibrationHolder GetLinearCalib(T *waveform);
        template <typename T> dataProducts::NonLinearCalibrationHolder GetNonLinearCalib(T *waveform);

        //methods for keeping data products in the tree
        bool KeepHeaders() { return keep_headers_; }
        bool KeepRawWaveforms() { return keep_raw_waveforms_; }
        bool KeepJitterCorrectedWaveforms() { return keep_jitter_corrected_waveforms_; }
        bool KeepDetectorWaveforms() { return keep_detector_waveforms_; }
        bool KeepODB() {return keep_odb_; }
        
        dataProducts::ChannelList GetChannelList(){ return channelList_; };

        bool GetDropEmptyTraces(){ return drop_empty_waveform_traces_; };
        template <typename T> bool DoEnergyCalibration(T *waveform);
        bool DoLysoEnergyCalibration(){return do_lyso_energy_calibration_;};
        bool DoHodoEnergyCalibration(){return do_hodo_energy_calibration_;};
        bool DoClustering(){return do_clustering_;};
        dataProducts::ClusterConfig GetClusterConfig(){ return clusterConfig_; };
        bool MakeHistograms(){return make_histograms_;};
        bool GetWriteEmptyHistograms(){return write_empty_histograms_;};
        bool DoPedestalCorrection(){return do_pedestal_jitter_correction_;};
        
        bool DoPeakFinding(){ return do_peak_finding_;};
        int GetPeakFindingAveragingWindow(){ return averaging_window_; };
        int GetPeakFindingSamplesBefore(){ return samples_before_; };
        int GetPeakFindingSamplesAfter(){ return samples_after_; };
        int GetPeakFindingDeadTime(){ return dead_time_; };

        bool GetInverted(dataProducts::ChannelID id) { return channelConfigMap_.at(id).inverted; };
        
        std::string GetDetectorType(dataProducts::ChannelID id) { return channelConfigMap_.at(id).detectorName; };
        int GetDetectorNum(dataProducts::ChannelID id) { return channelConfigMap_.at(id).detectorNum; };
        std::pair<double,double> GetPosition(dataProducts::ChannelID id) { return std::make_pair(channelConfigMap_.at(id).x,channelConfigMap_.at(id).y); };
        std::string GetConfigString(){return configString_;};

        dataProducts::RFConfig GetRFConfig(){return rf_config_;};

        bool DoSeededIntegral(){ return do_seeded_integration_;}
        dataProducts::SeededIntegralConfig GetSeededIntegralConfig(){ return seeded_integral_config_;}
        
        bool ReadRunSubRunFromFile() { return read_run_subrun_from_file_; }

        //Method to set the detector type and name in T
        template<typename T> void SetDetector(T &w);

        void FillChannelConfig(dataProducts::ChannelConfig& channelConfig, const boost::property_tree::ptree& pt, dataProducts::ChannelID channelID);

        std::vector<dataProducts::ChannelConfig> GetChannelConfigs();

        bool AlbedoMode() { return albedoMode_; }

    private:

        std::string inputFile_;
        std::string configString_;

        int run_;
        int subRun_;
        int event_;

        //reading run/subrun info
        bool read_run_subrun_from_file_;

        std::string spline_file_;
        dataProducts::WaveformFitterConfig fitter_config;
        bool keep_headers_;
        bool keep_raw_waveforms_;
        bool keep_jitter_corrected_waveforms_;
        bool keep_detector_waveforms_;
        bool keep_odb_;
        bool do_template_fit_;
        bool drop_empty_waveform_traces_;

        // energy calibration
        bool do_lyso_energy_calibration_;
        bool do_hodo_energy_calibration_;

        // clustering
        bool do_clustering_;

        // histogramming
        bool make_histograms_;
        bool write_empty_histograms_;

        // pedestal
        bool do_pedestal_jitter_correction_;

        // peak finding
        bool do_peak_finding_;
        int averaging_window_;
        int samples_before_;
        int samples_after_;
        int dead_time_;

        dataProducts::ChannelList channelList_;
        std::map<dataProducts::ChannelID,dataProducts::ChannelConfig> channelConfigMap_;

        dataProducts::ClusterConfig clusterConfig_;
        dataProducts::RFConfig rf_config_;

        bool do_seeded_integration_;
        dataProducts::SeededIntegralConfig seeded_integral_config_;

        std::map<dataProducts::ChannelID,std::vector<short>> traces_;

        bool albedoMode_;


    protected:


    };
}

#endif //SERIALIZERR_HH