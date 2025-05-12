#include "IntegralCluster.hh"
#include <numeric>


using namespace dataProducts;

IntegralCluster::IntegralCluster()
    : DataProduct(),
    eventNum(0),
    waveformIndex(0),
    amplitude(0.),
    fullintegral(0.),
    integral(0.),
    runNum(0),
    subRunNum(0),
    x(0),
    y(0)
{
}

template <typename T>
IntegralCluster::IntegralCluster(T* w, ClusterConfig config) : DataProduct()
    ,runNum(w->at(0).runNum)
    ,subRunNum(w->at(0).subRunNum)
    ,eventNum(w->at(0).eventNum)
    ,waveformIndex(w->at(0).waveformIndex)
    ,amplitude(0.)
    ,fullintegral(0.)
    ,integral(0.)
    ,x(0.)
    ,y(0.)
    ,is_clipping(false)
    // ,xtals(w) //not sure why this didn't work with just a normal TRef...
{
    DoClustering(w, config);
}

template <typename T>
void IntegralCluster::DoClustering(T* w, ClusterConfig config)
{
    double max_integral = -10000000;
    geom_mean = 1.0;
    geom_mean_secondaries = 1.0;

    double total_integral = 0;
    if (config.position_weighting_mode == 1)
    {
        // pre calculate the total integral
        for (auto &wi : *w)
        {
            total_integral += wi.integral;
        }
    }


    // std::cout << "INTEGRALCLUSTER::Computing positions in mode " << config.position_weighting_mode << std::endl;
    double total_position_weight = 0.0;
    double weight_i;
    for (auto &wi : *w)
    {
        amplitude    = amplitude + wi.amplitude;
        integral     = integral + wi.integral;
        fullintegral = fullintegral + wi.fullintegral;
        if (config.position_weighting_mode == 0)
        {
            // simple linear position weights
            x = x + wi.x * wi.integral; 
            y = y + wi.y * wi.integral; 
            total_position_weight += wi.integral;
        }
        else if (config.position_weighting_mode == 1)
        {
            // logarithmic position weights
            // see g-2 calo paper: https://arxiv.org/abs/1905.04407
            // or https://gm2-docdb.fnal.gov/cgi-bin/sso/ShowDocument?docid=3780
            weight_i = std::max(0.0, config.log_w + std::log(wi.integral / total_integral));
            // std::cout << "   -> Weighting crystal with fractional energy " << wi.integral/total_integral << " with " << weight_i << std::endl;
            x = x + wi.x * weight_i; 
            y = y + wi.y * weight_i; 
            total_position_weight += weight_i;
        }
        if(wi.is_clipping) is_clipping = true;
        geom_mean = geom_mean * wi.integral;
        // geom_mean_secondaries = geom_x * wi.integral;

        if (wi.integral > max_integral)
        {
            max_integral = wi.integral;
            max_channel = &wi;
        }
        xtals.Add(&wi); 
    }
    if(integral > 0)
    {
        // std::cout << "   -> Total weight " << total_position_weight << std::endl;
        x = x/total_position_weight;
        y = y/total_position_weight;

        if(w->size() > 1)
        {
            geom_mean_secondaries = std::pow(geom_mean/max_integral, 1.0/(w->size() - 1)) ;
        }
        else 
        {
            geom_mean_secondaries = -1;
        }
        geom_mean = std::pow(geom_mean, 1.0/w->size());

    }
    // std::cout << " -> total: " << integral << std::endl;
}

IntegralCluster::~IntegralCluster() {}

void IntegralCluster::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "IntegralCluster:" << std::endl;
    oss << "    run: " << runNum << std::endl;
    oss << "    subrun: " << subRunNum << std::endl;
    oss << "    event: " << eventNum << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << "    summed amplitude: " << amplitude << std::endl;
    oss << "    summed integral: " << integral << std::endl;
    oss << "    summed (full) integral: " << fullintegral << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}

template IntegralCluster::IntegralCluster(dataProducts::WaveformIntegralCollection* waveform, dataProducts::ClusterConfig config);
template IntegralCluster::IntegralCluster(dataProducts::CalibratedIntegralCollection* waveform, dataProducts::ClusterConfig config);
template void IntegralCluster::DoClustering(dataProducts::WaveformIntegralCollection* waveform, dataProducts::ClusterConfig config);
template void IntegralCluster::DoClustering(dataProducts::CalibratedIntegralCollection* waveform, dataProducts::ClusterConfig config);
