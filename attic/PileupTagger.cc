#include "PileupTagger.hh"
#include <numeric>


using namespace dataProducts;

PileupTagger::PileupTagger()
    : DataProduct()
    ,crateNum(0)
    ,amcNum(0)
    ,channelTag(0)
    ,eventNum(0)
    ,waveformIndex(0)
    ,npeaks(0)
    ,peak_times({})
    ,peak_amplitudes({})
    ,averaging_window(0)
    ,samples_before(0)
    ,samples_after(0)
    ,dead_time(0)
{
}

PileupTagger::PileupTagger(WaveformIntegral* w, int window, int before, int after, int dead) : DataProduct()
    ,runNum(w->runNum)
    ,subRunNum(w->subRunNum)
    ,crateNum(w->crateNum)
    ,amcNum(w->amcNum)
    ,channelTag(w->channelTag)
    ,eventNum(w->eventNum)
    ,waveformIndex(w->waveformIndex)
    ,integral(w)
    ,raw(w->raw)
    ,npeaks(0)
    ,peak_times({})
    ,peak_amplitudes({})
    ,averaging_window(window)
    ,samples_before(before)
    ,samples_after(after)
    ,dead_time(dead)
{
    FindPeaks();
}

void PileupTagger::FindPeaks()
{
    // tweaked implementation of alg 3 from here incorporating a rolling average
    // # https://www.sciencedirect.com/science/article/pii/S0010465510004182#fg0010
    npeaks = 0;
    peak_times.clear();
    peak_amplitudes.clear();


    // std::cout << "Tagging pileup" << std::endl;
    std::vector<double> result;
    Waveform *waveform = (Waveform*)raw.GetObject();
    // waveform->Show();
    float relative_prominance = waveform->pedestalStdev * 3;
    auto data = waveform->trace;
    for (int i = 0; i <= data.size() - averaging_window; ++i) {
        double sum = std::accumulate(data.begin() + i, data.begin() + i + averaging_window, 0.0);
        result.push_back(1.0*sum / averaging_window - waveform->pedestalLevel);
    }
    // std::cout << "Formed result vector with size " << result.size() << std::endl;
    
    float w1,w2,w3;
    for(int i = samples_before; i < result.size() - samples_after; i++)
    {
        // std::cout << "Looking for peak at index " << i << std::endl;
        w1 = result[i-samples_before];
        w2 = result[i];
        w3 = result[i+samples_after];
        // std::cout << "   -> " << i-samples_before << " / " << i << " / " << i+samples_after << " / " << std::endl;
        // std::cout << "   -> " << w1 << " / " << w2 << " / " << w3 << " / " << std::endl;
        if( w2-w1 > relative_prominance && w2-w3 > relative_prominance && (peak_times.size() == 0 || i > peak_times.back() + dead_time))
        {
            peak_times.push_back(i);
            peak_amplitudes.push_back(result[i]);
            npeaks++;
        }
    }

}

PileupTagger::~PileupTagger() {}

void PileupTagger::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "PileupTagger:" << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcSlot: " << amcNum << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}
