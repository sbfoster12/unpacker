#ifndef CHANNELCONFIG_HH 
#define CHANNELCONFIG_HH

#ifdef __ROOTCLING__
#pragma link C++ class std::tuple<int,int>+;
#endif

#include "DataProduct.hh"

namespace dataProducts {

    class ChannelConfig : public DataProduct {

        public:
            ChannelConfig();

            //Destructor
            ~ChannelConfig();

            std::string detectorName;
            int detectorNum;
            int crateNum;
            int amcSlotNum;
            int channelNum;
            double x;
            double y;
            double pedestal;
            double nSigmaIntegration;
            std::tuple<int,int> presamplesWindowSize;
            double integralSizeCutoff;
            double timingCalibration;
            double timingShift;
            bool isNonLinear;
            dataProducts::LinearCalibrationHolder linearCalib;
            dataProducts::NonLinearCalibrationHolder nonLinearCalib;
            bool doEnergyCalibration;
            bool inverted;

            double GetTimingCalibration();
            double GetTimingShift();
            void Show() const override;

            ClassDefOverride(ChannelConfig,2)
    };
    typedef std::vector<ChannelConfig> ChannelConfigCollection;
}

#endif // CHANNELCONFIG_HH