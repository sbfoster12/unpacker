#include "WaveformIntegral.hh"
#include <numeric>


using namespace dataProducts;

WaveformIntegral::WaveformIntegral()
    : DataProduct()
    ,crateNum(0)
    ,amcNum(0)
    ,channelTag(0)
    ,eventNum(0)
    ,waveformIndex(0)
    ,length(0)
    ,pedestalLevel(0.)
    ,pedestalStdev(0.)
    ,peak_time(0)
    ,amplitude(0.)
    ,fullintegral(0.)
    ,integral(0.)
    ,runNum(0)
    ,subRunNum(0)
    ,is_clipping(false)
    ,clipped_integration_window(false)
    ,x(0)
    ,y(0)
    ,detectorType("")
    ,detectorNum(0)
    ,is_time_corrected(false)
    ,time_shift(0)
    ,time_scale(0)
{
    integration_window = std::make_pair(0,0);
}

WaveformIntegral::WaveformIntegral(Waveform* waveform, double nsigma, int search_method, std::tuple<int,int> presample_config, int seed_index, int seeded_search_window) : DataProduct()
    ,runNum(waveform->runNum)
    ,subRunNum(waveform->subRunNum)
    ,crateNum(waveform->crateNum)
    ,amcNum(waveform->amcNum)
    ,channelTag(waveform->channelTag)
    ,eventNum(waveform->eventNum)
    ,waveformIndex(waveform->waveformIndex)
    ,length(waveform->length)
    ,pedestalLevel(waveform->pedestalLevel)
    ,pedestalStdev(waveform->pedestalStdev)
    ,peak_time(0)
    ,fullintegral(0)
    ,integral(0)
    ,nsigma(nsigma)
    ,search_method(search_method)
    ,raw(waveform)
    ,x(waveform->x)
    ,y(waveform->y)
    ,is_clipping(waveform->is_clipping)
    ,clipped_integration_window(false)
    ,detectorType(waveform->detectorType)
    ,detectorNum(waveform->detectorNum)
    ,is_time_corrected(false)
    ,time_shift(0)
    ,time_scale(0)
{
    DoIntegration(presample_config, seed_index, seeded_search_window);
}


void WaveformIntegral::DoIntegration(std::tuple<int,int> presample_config, int seed_index, int seeded_search_window)
{
    Waveform *waveform = (Waveform*)raw.GetObject();
    fullintegral = std::accumulate(waveform->trace.begin(), waveform->trace.end(), 0);
    fullintegral -= pedestalLevel*waveform->trace.size();  // remove the pedestal


    double threshold = pedestalLevel + waveform->pedestalStdev*nsigma;
    unsigned int index1 = 0;
    unsigned int index2 = waveform->trace.size();

    unsigned int max_peak_index;
    if(seed_index > 0)
    {
        if (seeded_search_window > 0)
        {
            auto search_bound_low = (seed_index - seeded_search_window > 0) ? std::begin(waveform->trace) + seed_index - seeded_search_window : std::begin(waveform->trace) ;
            auto search_bound_high = (seed_index + seeded_search_window < waveform->trace.size()) ? std::begin(waveform->trace) + seed_index + seeded_search_window : std::end(waveform->trace) ;
            max_peak_index = std::distance(std::begin(waveform->trace), std::max_element(search_bound_low, search_bound_high));
            // std::cout << "Searching for seeded peak in " << seed_index << " / " << seeded_search_window << std::endl;
        }
        else
        {
            max_peak_index = seed_index;
        }
        // std::cout << "Seeded max peak index:" << max_peak_index << std::endl;
    }
    else
    {
        max_peak_index = std::distance(std::begin(waveform->trace), std::max_element(std::begin(waveform->trace), std::end(waveform->trace)));
    }
    peak_time = max_peak_index;
    amplitude = waveform->trace.at(max_peak_index) - pedestalLevel;
    
    // int search_method = 1; //TODO: make this configurable
    if(search_method == 0)
    {
        // going from left to right, identify the first peak
        for (unsigned int i = 0; i < waveform->trace.size(); i++)
        {
            // std::cout << "   -> " << waveform->trace.at(i) << " / " <<  threshold << std::endl;
            if ((waveform->trace.at(i) > threshold) && (index1 == 0)) { index1 = i;}
            if ((index1 > 0) && (i > index1 + 10) && (waveform->trace.at(i) < threshold)) {index2 = i; break;}
        }
    }
    else if (search_method == 1)
    {
        // identify the highest peak, go out from there
        // Ternary operator -> (if condition) ? TRUE : FALSE
        // unsigned int peak_limit_low = (seeded_search_window > 0) ? peak_time + seeded_search_window : waveform->trace.size()
        for (unsigned int i = max_peak_index; i < waveform->trace.size(); i++)
        {
            if (i == 0) continue; //if peak is the leftmost index, skip first check
            // std::cout << "Searching for (high) at " << i << "   -> "  << waveform->trace.at(i) << " / " <<  threshold << std::endl;
            if(waveform->trace.at(i) < threshold && waveform->trace.at(i-1) < threshold ) {index2 = i; break;}
        }
        for (unsigned int i = max_peak_index; i > 0; i--)
        {
            // std::cout << "Searching for (low) at " << i << "   -> " << waveform->trace.at(i) << " / " <<  threshold << std::endl;
            if (i == (waveform->trace.size()-1)) continue; //if peak is the rightmost index, skip first check
            if(waveform->trace.at(i) < threshold && waveform->trace.at(i+1) < threshold ) {index1 = i; break;}
        }
    }
    else
    {
        std::cerr << "ERROR: Invalid search method in waveform integrator" << search_method << std::endl;
    }

    unsigned int min_presamples = std::get<0>(presample_config);
    unsigned int min_window_size = std::get<1>(presample_config);
    // std::cout << "working with pre/post samples: " << min_presamples << " / " << min_window_size << std::endl; 

    if( (max_peak_index-index1 < min_presamples) )
    {
        if((index1 > min_presamples)) 
        {
            index1 = max_peak_index - min_presamples;
        }
        else
        {
            index1 = 0;
            clipped_integration_window = true;
        }

    }
    // if((index2 - index1 < min_window_size) && (index1 + min_window_size < waveform->trace.size())) index2 = index1 + min_window_size;
    if(index2 - index1 < min_window_size) 
    {
        if (index1 + min_window_size < waveform->trace.size())
        {
            index2 = index1 + min_window_size;
        }
        else
        {
            index2 = waveform->trace.size()-1;
            clipped_integration_window = true;
        }
    }

    if(index2 >= waveform->trace.size() -1 || index1 == 0)  
        clipped_integration_window = true;
    // if(clipped_integration_window) 
    // {

    //     std::cout << "Integrating waveform of size " << waveform->trace.size() << " with pedestal " 
    //         << pedestalLevel << "+/-" << waveform->pedestalStdev << " between indices: " 
    //         << index1 << " / " << index2  << " (window " << index2 - index1 << ")" << std::endl;
    //     std::cout << "WARNING: integration window is clipping " << clipped_integration_window << std::endl;
    // }
    integral = std::accumulate(waveform->trace.begin()+index1, waveform->trace.begin()+index2, 0);
    integral -= pedestalLevel*(index2-index1);  // remove the pedestal
    integration_window = std::make_pair(index1,index2);

}

void WaveformIntegral::CorrectTimes()
{
    time_shift = ((Waveform*)(raw.GetObject()))->digitizationShift * ((Waveform*)(raw.GetObject()))->digitizationFrequency/40;
    peak_time = peak_time + time_shift;
    integration_window = std::make_pair(
        integration_window.first + time_shift,
        integration_window.second + time_shift
    );
    is_time_corrected = true;
}

ChannelID WaveformIntegral::GetID() const
{
    return std::make_tuple(crateNum, amcNum, channelTag);
}

WaveformIntegral::~WaveformIntegral() {}

void WaveformIntegral::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "WaveformIntegral:" << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcSlot: " << amcNum << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << "    detectorType: " << detectorType << std::endl;
    oss << "    detectorNum: " << detectorNum << std::endl;
    oss << "    pedestalLevel: " << pedestalLevel << std::endl;
    oss << "    peak time: " << peak_time << std::endl;
    oss << "    (restricted window) integral: " << integral << std::endl;
    oss << "    (full) integral: " << fullintegral << std::endl;
    oss << "    t0 time corrected: " << is_time_corrected << std::endl;
    oss << "    time offset (shift): " << time_shift << std::endl;
    oss << "    time offset (scale factor): " << time_scale << std::endl;
    oss << "        trace: ";
    for (const auto & sample : ((dataProducts::Waveform*)raw.GetObject())->trace) {
        oss << sample << ", ";
    }
    oss << std::endl;
    std::cout << oss.str();
}