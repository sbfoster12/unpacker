#include "Serializer.hh"

using namespace unpackers;

Serializer::Serializer() {}

Serializer::Serializer(std::string inputFile, int run, int subRun, int event) 
    : inputFile_(inputFile)
    ,run_(run)
    ,subRun_(subRun)
    ,event_(event)
{
    this->LoadChannelMapping();
}

Serializer::~Serializer() {}

void Serializer::SetRun(int run) { run_ = run; }
void Serializer::SetSubRun(int subRun) { subRun_ = subRun; }
void Serializer::SetEvent(int event) { event_ = event; }

std::string Serializer::GetString() const {
    nlohmann::json json_object = {
        {"run", run_}
        ,{"subRun", subRun_}
        ,{"event", event_}
    };

    std::vector<nlohmann::json> json_waveforms;
    for (auto& pair : traces_) {
        auto channelID = pair.first;

        nlohmann::json json_waveform;
        json_waveform["crateNum"] = std::get<0>(channelID);
        json_waveform["amcSlotNum"] = std::get<1>(channelID);
        json_waveform["channel"] = std::get<2>(channelID);
        json_waveform["channel_type"] = channelConfigMap_.at(channelID).detectorName;
        json_waveform["channel_id"] = channelConfigMap_.at(channelID).detectorNum;
        json_waveform["trace"] = pair.second;
        json_waveforms.push_back(json_waveform);
    }
    json_object["waveforms"] = json_waveforms;

    return json_object.dump();
}

void Serializer::LoadChannelMapping() {
    boost::property_tree::ptree pt;

    if (!std::filesystem::exists(inputFile_)) {
        std::cerr << "Error: Channel mapping file doesn't exist\n"
        << "File path = " << inputFile_ << "\n";
        exit(1);
    }

    try {
        boost::property_tree::read_json(inputFile_, pt);
    } catch (boost::property_tree::json_parser_error const &ex) {
        std::cerr << "Error: Error parsing JSON: " << ex.what() << std::endl;
        exit(1);
    }

    // store config for later dumping to root file
    std::ostringstream oss;
    boost::property_tree::json_parser::write_json(oss, pt);
    configString_ =  oss.str();

    int crateNum = 0;
    int nSlots = 12;
    int nChannels= 5;

    channelList_.clear();
    //loop over each slot & channel
    for (int slot = 1; slot <= nSlots; ++slot) {
        for (int channel = 0; channel < nChannels; ++channel) {
            auto channelID = std::make_tuple(crateNum,slot,channel);
            dataProducts::ChannelConfig channelConfig;
            this->FillChannelConfig(channelConfig,pt,channelID); //method here, so I don't need to include boost in the data products
            //channelConfig.Show();
            channelConfigMap_[channelID] = std::move(channelConfig);
            channelList_.push_back(channelID);
        }
    }

    // load if we should do energy/timing calibration
    do_lyso_energy_calibration_ = pt.get_child("integral_energy_calibration").get<bool>("do_lyso_calibration", true);
    do_hodo_energy_calibration_ = pt.get_child("integral_energy_calibration").get<bool>("do_hodo_calibration", true);

    //how to get run/subrun
    read_run_subrun_from_file_ = pt.get_child("inputs").get<bool>("read_run_subrun_from_file", false);

    // booleans to set whether to keep data products in the root tree or not
    keep_odb_ = pt.get_child("odb").get<bool>("keep_odb", true);
    keep_headers_ = pt.get_child("headers").get<bool>("keep_headers", true);
    keep_raw_waveforms_ = pt.get_child("waveforms").get<bool>("keep_raw_waveforms", false);
    keep_jitter_corrected_waveforms_ = pt.get_child("waveforms").get<bool>("keep_jitter_corrected_waveforms", false);
    keep_detector_waveforms_ = pt.get_child("waveforms").get<bool>("keep_detector_waveforms", true);

    // get whether to drop the empty traces
    drop_empty_waveform_traces_ = pt.get_child("waveforms").get<bool>("drop_empty_traces", false);

    // whether to do clustering, configuration
    do_clustering_ = pt.get_child("clustering").get<bool>("do_clustering", true);
    clusterConfig_ = {
        pt.get_child("clustering").get<int>("weighting", 0),
        pt.get_child("clustering").get<double>("log_w", 1.0),
    };

    // rf waveform configuration
    rf_config_ = {
        pt.get_child("rf").get<double>("frequency", 50.6e-3*6.28318530718),
        pt.get_child("rf").get<int>("amcNum"),
        pt.get_child("rf").get<int>("channelTag"),
        pt.get_child("rf").get<int>("nsamples",100),
        pt.get_child("rf").get<bool>("do_rf",true),
    };

    do_seeded_integration_ = pt.get_child("seeded_integration").get<bool>("do_seeded_integration", true);
    if(do_seeded_integration_)
    seeded_integral_config_ = {
        pt.get_child("seeded_integration").get<int>("trigger_low"),
        pt.get_child("seeded_integration").get<int>("trigger_high"),

    };

    // whether to do the histogram making, configuration
    make_histograms_ = pt.get_child("histograms").get<bool>("make_histograms", true);
    write_empty_histograms_ = pt.get_child("histograms").get<bool>("write_empty", false);

    // whether to do pedestal correction
    do_pedestal_jitter_correction_ = pt.get_child("pedestal_correction").get<bool>("do_pedestal_correction", true);

    // whether to do the peak finding
    do_peak_finding_ = pt.get_child("peak_finding").get<bool>("do_peak_finding", true);
    averaging_window_ = pt.get_child("peak_finding").get<int>("averaging_window");
    samples_before_ = pt.get_child("peak_finding").get<int>("samples_before");
    samples_after_ = pt.get_child("peak_finding").get<int>("samples_after");
    dead_time_ = pt.get_child("peak_finding").get<int>("dead_time");

    // load fitter configuration,probably a better way to do this...
    spline_file_ = pt.get_child("fits").get<std::string>("spline_file");
    do_template_fit_ = pt.get_child("fits").get<bool>("do_fits", true);
    fitter_config.clipCutLow_ = pt.get_child("fits").get<int>("clipCutLow");
    fitter_config.clipCutHigh_ = pt.get_child("fits").get<int>("clipCutHigh");
    fitter_config.wiggleRoom_ = pt.get_child("fits").get<unsigned int>("wiggleRoom");
    fitter_config.negPolarity_ = pt.get_child("fits").get<bool>("negPolarity");
    fitter_config.peakFitScaleThreshold_ = pt.get_child("fits").get<int>("peakFitScaleThreshold");
    fitter_config.peakCutoff_ = pt.get_child("fits").get<int>("peakCutoff");
    fitter_config.residualCutoff_ = pt.get_child("fits").get<double>("residualCutoff");
    fitter_config.ratioCutoff_ = pt.get_child("fits").get<double>("ratioCutoff");
    fitter_config.scaleCutoff_ = pt.get_child("fits").get<double>("scaleCutoff");
    fitter_config.artificialDeadTime_  = pt.get_child("fits").get<double>("artificialDeadTime");

    albedoMode_  = pt.get_child("experiment").get<bool>("albedo_mode");


}

void Serializer::SetWaveforms(const dataProducts::WaveformCollection& inputWaveforms) {
    traces_.clear();
    for (const auto& waveform : inputWaveforms) {
        auto channelID = std::make_tuple(waveform.crateNum,waveform.amcNum,waveform.channelTag);
        traces_[channelID] = waveform.trace;
    } // loop over waveforms
}

double Serializer::GetJitterCorrection(dataProducts::Waveform* w) {
    return channelConfigMap_.at(w->GetID()).pedestal;
}

double Serializer::GetNSigmaIntegration(dataProducts::Waveform* w) {
    return channelConfigMap_.at(w->GetID()).nSigmaIntegration;
}

std::tuple<int,int> Serializer::GetPresamplesWindowSize(dataProducts::Waveform* w) {
    return channelConfigMap_.at(w->GetID()).presamplesWindowSize;
}

double Serializer::GetIntegralSizeCutoff(dataProducts::Waveform* w) {
    return channelConfigMap_.at(w->GetID()).integralSizeCutoff;
}

void Serializer::FitterSetup(dataProducts::WaveformFitter* w) {
    w->LoadConfig(fitter_config);
}

template <typename T>
double Serializer::GetTimingCalibration(T *w) {
    return channelConfigMap_.at(w->GetID()).GetTimingCalibration();
}

template <typename T>
double Serializer::GetTimingShift(T *w) {
    return channelConfigMap_.at(w->GetID()).GetTimingShift();
}

template <typename T>
bool Serializer::IsNonLinear(T *w) {
        return  channelConfigMap_.at(w->GetID()).isNonLinear;
    }

template <typename T>
dataProducts::LinearCalibrationHolder Serializer::GetLinearCalib(T *w) {
    return channelConfigMap_.at(w->GetID()).linearCalib;
}

template <typename T>
dataProducts::NonLinearCalibrationHolder Serializer::GetNonLinearCalib(T *w) {
    return channelConfigMap_.at(w->GetID()).nonLinearCalib;
}

template <typename T>
bool Serializer::DoEnergyCalibration(T *w) {
    return channelConfigMap_.at(w->GetID()).doEnergyCalibration;
}

template<typename T>
void Serializer::SetDetector(T& w) {
    w.detectorType = channelConfigMap_.at(w.GetID()).detectorName;
    w.detectorNum = channelConfigMap_.at(w.GetID()).detectorNum;
}

void Serializer::FillChannelConfig(dataProducts::ChannelConfig& channelConfig, const boost::property_tree::ptree& pt, dataProducts::ChannelID channelID) {

    channelConfig.crateNum = std::get<0>(channelID);
    channelConfig.amcSlotNum =std::get<1>(channelID);
    channelConfig.channelNum =std::get<2>(channelID);
    channelConfig.nSigmaIntegration = pt.get_child("integrals").get<double>("nsigma");
    channelConfig.presamplesWindowSize = {pt.get_child("integrals").get<double>("min_presamples"),pt.get_child("integrals").get<double>("min_window_size")};
    channelConfig.integralSizeCutoff = pt.get_child("integrals").get<double>("integral_cutoff");

    //detector channel mapping
    bool found = false;
    for (const auto& detectorType : pt.get_child("detectors")) {
        for (const auto& detector : detectorType.second) {

            int thisCrateNum = detector.second.get<int>("crateNum");
            int thisSlot = detector.second.get<int>("amcSlotNum");
            int thisChannel = detector.second.get<int>("channelNum");
            auto thisChannelID = std::make_tuple(thisCrateNum,thisSlot,thisChannel);
            if (channelID != thisChannelID) { continue; }

            //If here, channel ID matches
            if (found) {
                std::cerr << "Error: Provided detector mapping has a duplicate entry for\n"
                << "crateNum=" << thisCrateNum << ", amcNum=" << thisSlot << ", channelNum=" << thisChannel << "\n";
                exit(1);
            }

            found=true;
            channelConfig.crateNum = thisCrateNum;
            channelConfig.amcSlotNum = thisSlot;
            channelConfig.channelNum = thisChannel;
            channelConfig.detectorName = detector.second.get<std::string>("detectorName");
            channelConfig.detectorNum = detector.second.get<int>("detectorNum");
            channelConfig.x = detector.second.get<double>("x", 0.0);
            channelConfig.y = detector.second.get<double>("y", 0.0);

            std::cout << "Message: Loading detector mapping for "
            << "crate=" << channelConfig.crateNum << ", slot=" << channelConfig.amcSlotNum << ", channel=" <<  channelConfig.channelNum
            << ": detectorName=" << channelConfig.detectorName << ", detectorNum=" << channelConfig.detectorNum << std::endl;

        } // end loop over specific detectors
    } // end loop over detector types

    if (!found) {
        std::cout << "Warning: No detector mapping found for "
        << "crate=" << std::get<0>(channelID) << ", slot=" << std::get<1>(channelID) << ", channel=" <<  std::get<2>(channelID) << std::endl;
    }


    //Find the pedestal map for this slot and channel
    found = false;
    for (const auto& entry : pt.get_child("pedestals")) {

        int thisCrateNum = entry.second.get<int>("crateNum");
        int thisSlot = entry.second.get<int>("amcSlotNum");
        int thisChannel = entry.second.get<int>("channelNum");
        auto thisChannelID = std::make_tuple(thisCrateNum,thisSlot,thisChannel);
        if (channelID != thisChannelID) { continue; }

        //if we are here, then we found a match!
        if (found) {
            std::cerr << "Error: Provided pedestal mapping has a duplicate entry for\n"
            << "crateNum=" << channelConfig.crateNum << ", amcNum=" << channelConfig.amcSlotNum << ", channelNum=" << channelConfig.channelNum << "\n";
            exit(1);
        }

        found = true;
        channelConfig.pedestal = entry.second.get<double>("pedestal",0.0);
        channelConfig.nSigmaIntegration = entry.second.get<double>("nsigma", channelConfig.nSigmaIntegration);
        channelConfig.integralSizeCutoff = entry.second.get<double>("integral_cutoff", channelConfig.integralSizeCutoff);
        channelConfig.inverted = entry.second.get<bool>("inverted", false);
        double this_min_presamples = entry.second.get<int>("min_presamples", std::get<0>(channelConfig.presamplesWindowSize));
        double this_min_window_size = entry.second.get<int>("min_window_size", std::get<1>(channelConfig.presamplesWindowSize));
        channelConfig.presamplesWindowSize = {this_min_presamples,this_min_window_size};
    } // loop over pedestal configurations

    if (!found) {
        std::cout << "Warning: No pedestal mapping found for "
        << "crate=" << std::get<0>(channelID) << ", slot=" << std::get<1>(channelID) << ", channel=" <<  std::get<2>(channelID) << std::endl;
    }

    //calibration
    if (channelConfig.detectorName == "lyso") {
        channelConfig.doEnergyCalibration = pt.get_child("integral_energy_calibration").get<bool>("do_lyso_calibration", true);
    } else if ((channelConfig.detectorName == "hodo_x") || (channelConfig.detectorName == "hodo_y")){
        channelConfig.doEnergyCalibration = pt.get_child("integral_energy_calibration").get<bool>("do_hodo_calibration", true);
    } else {
        channelConfig.doEnergyCalibration = false;
    }

    found = false;
    //find the calibration for this detector
    for (const auto& detectors : pt.get_child("calibration")) {
        //check if detector type matches
        if (detectors.first != channelConfig.detectorName) { continue; }

        //detector type matches, so loop over specific detectors
        for (const auto& detector : detectors.second) {

            //check if specific detector matches
            if ( !( (channelConfig.detectorName == detector.second.get<std::string>("detectorName"))
                & (channelConfig.detectorNum == detector.second.get<int>("detectorNum"))
                ) ) { continue; }

            //if we are here, we found a match!
            if (found) {
                std::cerr << "Error: Provided calibration mapping has a duplicate entry for\n"
                << "detectorName=" << channelConfig.detectorName << ", detectorNum=" << channelConfig.detectorNum << ", channelNum=" << channelConfig.channelNum << "\n";
                exit(1);
            }

            found = true;

            channelConfig.isNonLinear = detector.second.get<bool>("non_linear",false);
            if(channelConfig.isNonLinear) {
                int npar = detector.second.get<double>("npar",0);
                std::vector<double> params;
                for (int i = 0; i < npar; i++) {
                    params.push_back(
                        detector.second.get<double>(Form("p%i", i)));
                }

                auto nonLinearCalib = dataProducts::NonLinearCalibrationHolder(
                    channelConfig.pedestal,
                    npar,
                    params,
                    detector.second.get<std::string>("function_name"),
                    detector.second.get<double>("time_calib",1.0),
                    detector.second.get<double>("time_offset",0.0)
                );
                channelConfig.nonLinearCalib = std::move(nonLinearCalib);
            } else  {
                auto linearCalib  = dataProducts::LinearCalibrationHolder(
                    channelConfig.pedestal,
                    detector.second.get<double>("energy_calib",1.0),
                    detector.second.get<double>("time_calib",1.0),
                    detector.second.get<double>("time_offset",0.0)
                );
                channelConfig.linearCalib = std::move(linearCalib);
            } // end if linear

        } //end loop over detectors
    } //end loop over detector types

    if (!found) {
        std::cout << "Warning: No calibration mapping found for "
        << "detectorName=" << channelConfig.detectorName << ", detectorNum=" << channelConfig.detectorNum << std::endl;
    }
}

std::vector<dataProducts::ChannelConfig> Serializer::GetChannelConfigs() {
    std::vector<dataProducts::ChannelConfig> output;
    for (auto val : channelConfigMap_) {
        output.push_back(val.second);
    }
    return output;
}

// template double Serializer::GetEnergyCalibration<dataProducts::Waveform>(dataProducts::Waveform* waveform);
template double Serializer::GetTimingCalibration<dataProducts::Waveform>(dataProducts::Waveform* waveform);
template double Serializer::GetTimingShift<dataProducts::Waveform>(dataProducts::Waveform* waveform);
template bool Serializer::DoEnergyCalibration<dataProducts::Waveform>(dataProducts::Waveform* waveform);
template void Serializer::SetDetector<dataProducts::Waveform>(dataProducts::Waveform &waveform);
template dataProducts::LinearCalibrationHolder Serializer::GetLinearCalib<dataProducts::Waveform>(dataProducts::Waveform* waveform);
template dataProducts::NonLinearCalibrationHolder Serializer::GetNonLinearCalib<dataProducts::Waveform>(dataProducts::Waveform* waveform);
template bool Serializer::IsNonLinear<dataProducts::Waveform>(dataProducts::Waveform* waveform);

// template double Serializer::GetEnergyCalibration<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral* waveform);
template double Serializer::GetTimingCalibration<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral* waveform);
template double Serializer::GetTimingShift<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral* waveform);
template bool Serializer::DoEnergyCalibration<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral* waveform);
template void Serializer::SetDetector<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral &waveform);
template dataProducts::LinearCalibrationHolder Serializer::GetLinearCalib<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral* waveform);
template dataProducts::NonLinearCalibrationHolder Serializer::GetNonLinearCalib<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral* waveform);
template bool Serializer::IsNonLinear<dataProducts::WaveformIntegral>(dataProducts::WaveformIntegral* waveform);

// template double Serializer::GetEnergyCalibration<dataProducts::WaveformFit>(dataProducts::WaveformFit* waveform);
template double Serializer::GetTimingCalibration<dataProducts::WaveformFit>(dataProducts::WaveformFit* waveform);
template double Serializer::GetTimingShift<dataProducts::WaveformFit>(dataProducts::WaveformFit* waveform);
template bool Serializer::DoEnergyCalibration<dataProducts::WaveformFit>(dataProducts::WaveformFit* waveform);
template void Serializer::SetDetector<dataProducts::WaveformFit>(dataProducts::WaveformFit &waveform);
template dataProducts::LinearCalibrationHolder Serializer::GetLinearCalib<dataProducts::WaveformFit>(dataProducts::WaveformFit* waveform);
template dataProducts::NonLinearCalibrationHolder Serializer::GetNonLinearCalib<dataProducts::WaveformFit>(dataProducts::WaveformFit* waveform);
template bool Serializer::IsNonLinear<dataProducts::WaveformFit>(dataProducts::WaveformFit* waveform);
