#include "ChannelConfig.hh"

using namespace dataProducts;

ChannelConfig::ChannelConfig()
    : DataProduct(),
    detectorName("NaN"),
    detectorNum(0),
    crateNum(0),
    amcSlotNum(0),
    channelNum(0),
    x(0),
    y(0),
    pedestal(0),
    nSigmaIntegration(0),
    presamplesWindowSize({0,0}),
    integralSizeCutoff(0),
    timingCalibration(1),
    timingShift(0),
    isNonLinear(false),
    linearCalib(),
    nonLinearCalib(),
    doEnergyCalibration(false),
    inverted(false)
{}

double ChannelConfig::GetTimingCalibration() {
    if (isNonLinear) {
        return nonLinearCalib.time_calib;
    } else {
        return linearCalib.time_calib;
    }
}

double ChannelConfig::GetTimingShift() {
    if (isNonLinear) {
        return nonLinearCalib.time_shift;
    } else {
        return linearCalib.time_shift;
    }
}

void ChannelConfig::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "ChannelConfig:" << std::endl;
    oss << "    detectorName: " << detectorName << std::endl;
    oss << "    detectorNum: " << detectorNum << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcSlot: " << amcSlotNum << std::endl;
    oss << "    channelTag: " << channelNum << std::endl;
    oss << "    x: " << x << std::endl;
    oss << "    y: " << y << std::endl;
    oss << "    pedestal: " << pedestal << std::endl;
    oss << "    nSigmaIntegration: " << nSigmaIntegration << std::endl;
    oss << "    presamplesWindowSize: " << std::get<0>(presamplesWindowSize) << ", " << std::get<1>(presamplesWindowSize) << std::endl;
    oss << "    integralSizeCutoff: " << integralSizeCutoff << std::endl;
    oss << "    timingCalibration: " << timingCalibration << std::endl;
    oss << "    timingShift: " << timingShift << std::endl;
    oss << "    isNonLinear: " << isNonLinear << std::endl;
    oss << "    doEnergyCalibration: " << doEnergyCalibration << std::endl;
    if (!isNonLinear) {
    oss << "    linearCalib: " << std::endl;
    oss << "        pedestal: " << linearCalib.pedestal << std::endl;
    oss << "        energy_calib: " << linearCalib.energy_calib << std::endl;
    oss << "        time_calib: " << linearCalib.time_calib << std::endl;
    oss << "        time_shift: " << linearCalib.time_shift << std::endl;
    } else {
    oss << "    nonLinearCalib: " << doEnergyCalibration << std::endl;
    oss << "        pedestal: " << nonLinearCalib.pedestal << std::endl;
    oss << "        energy_calib_params: " << std::endl;
    for (uint i = 0; i < nonLinearCalib.energy_calib_params.size(); ++i) {
    oss << "        p[0" << i << "] = " << nonLinearCalib.energy_calib_params[i] << std::endl;
    }
    oss << std::endl;
    oss << "        function_name: " << nonLinearCalib.function_name << std::endl;
    oss << "        time_calib: " << nonLinearCalib.time_calib << std::endl;
    oss << "        time_shift: " << nonLinearCalib.time_shift << std::endl;
    }
    oss << std::endl;
    std::cout << oss.str();
}

ChannelConfig::~ChannelConfig() {}