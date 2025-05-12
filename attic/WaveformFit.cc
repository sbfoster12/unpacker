#include "WaveformFit.hh"
#include <numeric>


using namespace dataProducts;

WaveformFit::WaveformFit()
    : DataProduct(),
    crateNum(0),
    amcNum(0), 
    channelTag(0),
    eventNum(0),
    waveformIndex(0),
    length(0),
    pedestalLevel(0.),
    runNum(0),
    subRunNum(0),
    detectorType(""),
    detectorNum(0)
{
}

WaveformFit::WaveformFit(Waveform* wf) : DataProduct()
    ,runNum(wf->runNum)
    ,subRunNum(wf->subRunNum)
    ,crateNum(wf->crateNum)
    ,amcNum(wf->amcNum)
    ,channelTag(wf->channelTag)
    ,eventNum(wf->eventNum)
    ,waveformIndex(wf->waveformIndex)
    ,length(wf->length)
    ,pedestalLevel(wf->pedestalLevel)
    ,waveform(wf)
    ,chi2(-1)
    ,converged(false)
    ,ndf(1)
    ,nfit(0)
    ,times({})
    ,amplitudes({})
    ,detectorType(wf->detectorType)
    ,detectorNum(wf->detectorNum)
{
    // std::cout << "Created empty waveform fit result" << std::endl;
    // Show();
}

WaveformFit::~WaveformFit() {}

ChannelID WaveformFit::GetID() const {
    return std::make_tuple(crateNum, amcNum, channelTag);
}

void WaveformFit::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "WaveformFit:" << std::endl;
    oss << "    run: " << runNum << std::endl;
    oss << "    subrun: " << subRunNum << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcSlot: " << amcNum << std::endl;
    oss << "    detectorType: " << detectorType << std::endl;
    oss << "    detectorNum: " << detectorNum << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    waveformIndex: " << waveformIndex << std::endl;
    oss << "    pedestalLevel: " << pedestalLevel << std::endl;
    oss << "    converged: " << converged << std::endl;
    oss << "    chi2: " << chi2 << std::endl;
    oss << "    ndf: " << ndf << std::endl;
    oss << "    nfit: " << nfit << std::endl;
    for (int i = 0; i < nfit; i++)
    {
        oss << "   -> pulse " << i+1 << " energy/time = " << amplitudes.at(i) << " / " << times.at(i) << std::endl;
    }
    oss << std::endl;
    std::cout << oss.str();
}