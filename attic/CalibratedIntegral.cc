#include "CalibratedIntegral.hh"
#include <numeric>


using namespace dataProducts;

CalibratedIntegral::CalibratedIntegral()
    : DataProduct(),
    crateNum(0),
    amcNum(0), 
    channelTag(0),
    eventNum(0),
    waveformIndex(0),
    length(0),
    pedestalLevel(0.),
    pedestalStdev(0.),
    amplitude(0.),
    fullintegral(0.),
    integral(0.),
    runNum(0),
    subRunNum(0),
    is_clipping(false),
    energy_calib(1.0),
    time_calib(1.0),
    time_shift(0.0),
    x(0.),
    y(0.),
    non_linear(false),
    detectorType(""),
    detectorNum(0)
{
    integration_window = std::make_pair(0,0);
}

CalibratedIntegral::CalibratedIntegral(WaveformIntegral* w, double e, double t_calib, double t_offset) : DataProduct()
    ,runNum(w->runNum)
    ,subRunNum(w->subRunNum)
    ,crateNum(w->crateNum)
    ,amcNum(w->amcNum)
    ,channelTag(w->channelTag)
    ,eventNum(w->eventNum)
    ,waveformIndex(w->waveformIndex)
    ,length(w->length)
    ,pedestalLevel(w->pedestalLevel)
    ,pedestalStdev(w->pedestalStdev)
    ,amplitude(w->amplitude*e)
    ,fullintegral(w->fullintegral*e)
    ,integral(w->integral*e)
    ,nsigma(w->nsigma)
    ,search_method(w->search_method)
    ,integration_window(w->integration_window)
    ,uncalibrated(w)
    ,raw(w->raw)
    ,x(w->x)
    ,y(w->y)
    ,non_linear(false)
    ,is_clipping(w->is_clipping)
    ,detectorType(w->detectorType)
    ,detectorNum(w->detectorNum)
{
    // DoCalibration()
    energy_calib = e;
    time_calib = t_calib;
    time_shift = t_offset;
    // std::cout << "Creating a calibrated integral with the energy/t_calib/t_offset: " <<
    //     energy_calib << " / " << 
    //     time_calib << " /  " << 
    //     time_shift << std::endl;
}

CalibratedIntegral::CalibratedIntegral(WaveformIntegral* w, LinearCalibrationHolder map) : DataProduct()
    ,runNum(w->runNum)
    ,subRunNum(w->subRunNum)
    ,crateNum(w->crateNum)
    ,amcNum(w->amcNum)
    ,channelTag(w->channelTag)
    ,eventNum(w->eventNum)
    ,waveformIndex(w->waveformIndex)
    ,length(w->length)
    ,pedestalLevel(w->pedestalLevel)
    ,pedestalStdev(w->pedestalStdev)
    ,amplitude(w->amplitude*map.energy_calib)
    ,fullintegral(w->fullintegral*map.energy_calib)
    ,integral(w->integral*map.energy_calib)
    ,nsigma(w->nsigma)
    ,search_method(w->search_method)
    ,integration_window(w->integration_window)
    ,peak_time(w->peak_time*map.time_calib + map.time_shift)
    ,uncalibrated(w)
    ,raw(w->raw)
    ,x(w->x)
    ,y(w->y)
    ,non_linear(false)
    ,is_clipping(w->is_clipping)
    ,detectorType(w->detectorType)
    ,detectorNum(w->detectorNum)
{
    // std::cout << "performing linear calibration "<< std::endl;
    // DoCalibration()
    energy_calib = map.energy_calib;
    time_calib = map.time_calib;
    time_shift = map.time_shift;

    integration_window.first += time_shift;
    integration_window.second += time_shift;
    // std::cout << "Creating a calibrated integral with the energy/t_calib/t_offset: " <<
    //     energy_calib << " / " << 
    //     time_calib << " /  " << 
    //     time_shift << std::endl;
    // std::cout << "integration window: " << integration_window.first << " / " << integration_window.second << std::endl;
}

CalibratedIntegral::CalibratedIntegral(WaveformIntegral* w, NonLinearCalibrationHolder map) : DataProduct()
    ,runNum(w->runNum)
    ,subRunNum(w->subRunNum)
    ,crateNum(w->crateNum)
    ,amcNum(w->amcNum)
    ,channelTag(w->channelTag)
    ,eventNum(w->eventNum)
    ,waveformIndex(w->waveformIndex)
    ,length(w->length)
    ,pedestalLevel(w->pedestalLevel)
    ,pedestalStdev(w->pedestalStdev)
    ,amplitude(w->amplitude)
    ,fullintegral(w->fullintegral)
    ,integral(w->integral)
    ,nsigma(w->nsigma)
    ,search_method(w->search_method)
    ,integration_window(w->integration_window)
    ,peak_time(w->peak_time*map.time_calib + map.time_shift)
    ,uncalibrated(w)
    ,raw(w->raw)
    ,x(w->x)
    ,y(w->y)
    ,non_linear(true)
    ,is_clipping(w->is_clipping)
    ,detectorType(w->detectorType)
    ,detectorNum(w->detectorNum)
{
    DoNonLinearCalibration(map);
    // energy_calib = map.energy_calib;
    // time_calib = map.time_calib;
    // time_shift = map.time_shift;

    // integration_window.first += time_shift;
    // integration_window.second += time_shift;
    // std::cout << "Creating a calibrated integral with the energy/t_calib/t_offset: " <<
    //     energy_calib << " / " << 
    //     time_calib << " /  " << 
    //     time_shift << std::endl;
    // std::cout << "integration window: " << integration_window.first << " / " << integration_window.second << std::endl;
}

void CalibratedIntegral::DoNonLinearCalibration(NonLinearCalibrationHolder map)
{
    // TODO: implement function for nl calibration
    std::cout << "performing non-linear calibration..." << std::endl;
}

CalibratedIntegral::~CalibratedIntegral() {}

void CalibratedIntegral::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "CalibratedIntegral:" << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcSlot: " << amcNum << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    detectorType: " << detectorType << std::endl;
    oss << "    detectorNum: " << detectorNum << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << "    pedestalLevel: " << pedestalLevel << std::endl;
    oss << "    energy calibration constant: " << energy_calib << std::endl;
    oss << "    timing calibration constant: " << time_calib << std::endl;
    oss << "    timing offset: " << time_shift << std::endl;
    oss << "    peak time: " << peak_time << std::endl;
    oss << "    (restricted window) integral: " << integral << std::endl;
    oss << "    (full) integral: " << fullintegral << std::endl;
    oss << "        trace: ";
    for (const auto & sample : ((dataProducts::Waveform*)raw.GetObject())->trace) {
        oss << sample << ", ";
    }
    oss << std::endl;
    std::cout << oss.str();
}
