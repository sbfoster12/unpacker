#include "RFWaveform.hh"
#include <numeric>

using namespace dataProducts;

RFWaveform::RFWaveform()
    : DataProduct(),
    crateNum(0),
    amcNum(0),
    channelTag(0),
    eventNum(0),
    waveformIndex(0),
    pedestalLevel(0.),
    pedestalStdev(0.),
    is_clipping(false)
    , frequency(0)
    , phase(0)
    , amplitude(0)
    , constant(0)
    , corrected_phase(-10)
    , t0_in_trigger_window(false)
{
}

RFWaveform::RFWaveform(Waveform *waveform, dataProducts::RFConfig config) : DataProduct()
    , runNum(waveform->runNum)
    , subRunNum(waveform->subRunNum)
    , crateNum(waveform->crateNum)
    , amcNum(waveform->amcNum)
    , channelTag(waveform->channelTag)
    , waveformIndex(waveform->waveformIndex)
    , eventNum(waveform->eventNum)
    , pedestalLevel(waveform->pedestalLevel)
    , pedestalStdev(waveform->pedestalStdev)
    , raw(waveform)
    , is_clipping(waveform->is_clipping)
    , frequency(0)
    , phase(0)
    , amplitude(0)
    , constant(0)
    , corrected_phase(-10)
    , t0_in_trigger_window(false)
{
    frequency = config.frequency;
    IdentifyPhase(config);
    // std::cout << "Found waveform with phase: " << phase << std::endl;
}

void RFWaveform::FitSine(std::vector<short> *trace, std::vector<float> *wfTime, double &ampl, double &phi, double &constant, int nsamples)
{
    // copied from Patrick S. wavedream analysis code
    // fitting the amplitude/phase of a fixed RF frequency from PSI
    // int nsamples = 100;

    TMatrixD M(3, 3);
    TVectorD v(3);

    for (int i = 0; i < nsamples; ++i)
    {
        // float co = TMath::Cos(frequency * wfTime->at(i));
        // float si = TMath::Sin(frequency * wfTime->at(i));
        float co = TMath::Cos(frequency * i*1.25*6.28318530718);
        float si = TMath::Sin(frequency * i*1.25*6.28318530718);

        M[0][0] += co * co;
        M[0][1] += co * si;
        M[0][2] += co;
        M[1][1] += si * si;
        M[1][2] += si;
        M[2][2] += 1;
        v[0] += co * 1.0*trace->at(i);
        v[1] += si * 1.0*trace->at(i);
        v[2] += 1.0*trace->at(i);
    }
    M[1][0] = M[0][1];
    M[2][0] = M[0][2];
    M[2][1] = M[1][2];

    M.Invert();
    v = M * v;

    amplitude = TMath::Sqrt(v[0] * v[0] + v[1] * v[1]);
    phase  = TMath::ATan2(v[0], v[1]);
    constant = v[2];
}

void RFWaveform::IdentifyPhase(RFConfig config)
{
    Waveform *waveform = (Waveform *)raw.GetObject();
    FitSine(&(waveform->trace), NULL, amplitude, phase, pedestalLevel, config.nsamples);
}


void RFWaveform::CorrectPhaseT0(WaveformIntegral *t0, int trigger_window_low, int trigger_window_high)
{
    double t0_time = t0->peak_time;
    if(t0_time > trigger_window_low && t0_time < trigger_window_high)
    {
        t0_in_trigger_window = true;
    }
    else 
    {
        std::cout << "Warning: T0 peak does not occur in the trigger range (" << trigger_window_low 
                  << " - " << trigger_window_high << ") for event " << t0->eventNum << " / " << t0->waveformIndex << std::endl;
    }
    // TODO: this line assumes 800MHz for t0, we can get this from the data instead
    double t0_in_rf_phase = (t0_time * 1.25)*(frequency)*(2*TMath::Pi());
    corrected_phase = fmod(phase + t0_in_rf_phase, 2.0*TMath::Pi());
}

ChannelID RFWaveform::GetID()
{
    return std::make_tuple(crateNum, amcNum, channelTag);
}

RFWaveform::~RFWaveform() {}

void RFWaveform::Show() const
{
    std::ostringstream oss;
    oss << std::endl;
    oss << "RFWaveform:" << std::endl;
    oss << "    crateNum: " << crateNum << std::endl;
    oss << "    amcSlot: " << amcNum << std::endl;
    oss << "    channelTag: " << channelTag << std::endl;
    oss << "    eventNum: " << eventNum << std::endl;
    oss << "    pedestalLevel: " << pedestalLevel << std::endl;
    oss << "    frequency: " << frequency << std::endl;
    oss << "    phase: " << phase << std::endl;
    oss << "    amplitude: " << amplitude << std::endl;
    oss << "    constant: " << constant << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}