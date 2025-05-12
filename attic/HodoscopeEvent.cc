#include "HodoscopeEvent.hh"
#include <numeric>


using namespace dataProducts;

HodoscopeEvent::HodoscopeEvent() : DataProduct()
    ,runNum(0)
    ,subRunNum(0)
    ,eventNum(0)
    ,waveformIndex(0)
    ,amplitude_x(0.)
    ,fullintegral_x(0.)
    ,integral_x(0.)
    ,amplitude_y(0.)
    ,fullintegral_y(0.)
    ,integral_y(0.)
    ,x(0)
    ,y(0)
    ,nx(0)
    ,ny(0)
    ,containsNegativeIntegrals(false)
{
}

template <typename T>
HodoscopeEvent::HodoscopeEvent(T* xs, T* ys) : DataProduct()
    ,runNum(0)
    ,subRunNum(0)
    ,eventNum(0)
    ,waveformIndex(0)
    ,amplitude_x(0.)
    ,fullintegral_x(0.)
    ,integral_x(0.)
    ,amplitude_y(0.)
    ,fullintegral_y(0.)
    ,integral_y(0.)
    ,x(0.)
    ,y(0.)
    ,nx(xs->size())
    ,ny(ys->size())
    ,containsNegativeIntegrals(false)
{
    // TODO: add x/y position data
    max_integral_x = -10000000;
    max_integral_y = -10000000;
    // std::cout << "creating integral (starting from " << integral << " of course)" << std::endl;
    for (auto &wi : *xs)
    {
        //check if the integral is negative (bad waveform)
        if (wi.integral < 0) containsNegativeIntegrals = true;

        // std::cout << "    -> adding: " << wi.integral << std::endl;
        amplitude_x    = amplitude_x + wi.amplitude;
        integral_x     = integral_x + wi.integral;
        fullintegral_x = fullintegral_x + wi.fullintegral;
        x = x + wi.x*wi.integral;

        if (wi.integral > max_integral_x)
        {
            max_x = wi.x;
            max_integral_x = wi.integral;
            max_channel_x = wi.channelTag;
            max_amc_x = wi.amcNum;
            max_crate_x = wi.crateNum;
        }
        hodo_x.Add(&wi); 
    }
    if(integral_x > 0) x = x/integral_x;

    for (auto &wi : *ys)
    {
        //check if the integral is negative (bad waveform)
        if (wi.integral < 0) containsNegativeIntegrals = true;

        // std::cout << "    -> adding: " << wi.integral << std::endl;
        amplitude_y    = amplitude_y + wi.amplitude;
        integral_y     = integral_y + wi.integral;
        fullintegral_y = fullintegral_y + wi.fullintegral;
        y = y + wi.y*wi.integral;

        if (wi.integral > max_integral_y)
        {
            max_y = wi.y;
            max_integral_y = wi.integral;
            max_channel_y = wi.channelTag;
            max_amc_y = wi.amcNum;
            max_crate_y = wi.crateNum;
        }
        hodo_y.Add(&wi); 
    }
    if(integral_y > 0) y = y/integral_y;
    // std::cout << " -> total: " << integral << std::endl;
}

HodoscopeEvent::~HodoscopeEvent() {}

void HodoscopeEvent::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "HodoscopeEvent:" << std::endl;
    oss << "    run: " << runNum << std::endl;
    oss << "    subrun: " << subRunNum << std::endl;
    oss << "    event: " << eventNum << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << "    summed amplitude: " << amplitude_x << " / " << amplitude_y << std::endl;
    oss << "    summed integral: " << integral_x << " / " << integral_y << std::endl;
    oss << "    summed (full) integral: " << fullintegral_x << " / " << fullintegral_y << std::endl;
    oss << "    contains negative Integrals " << containsNegativeIntegrals << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}

template HodoscopeEvent::HodoscopeEvent(dataProducts::WaveformIntegralCollection* xs, dataProducts::WaveformIntegralCollection* ys);
template HodoscopeEvent::HodoscopeEvent(dataProducts::CalibratedIntegralCollection* xs, dataProducts::CalibratedIntegralCollection* ys);
