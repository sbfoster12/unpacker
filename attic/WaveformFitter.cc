#include "WaveformFitter.hh"
#include <numeric>
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"

using namespace dataProducts;

SingleWaveformFitter::SingleWaveformFitter() : DataProduct()
{
}

SingleWaveformFitter::~SingleWaveformFitter(){};

SingleWaveformFitter::SingleWaveformFitter(TSpline3 sp) : DataProduct(), spline(sp), nfits(1)
{
}

WaveformFitter::WaveformFitter()
    : DataProduct()
{
}

WaveformFitter::WaveformFitter(std::string sp, ChannelList ch, bool preload_splines) : DataProduct()
    ,spline_file(sp)
    ,channels(ch)
    ,clipCutLow_(-2000)
    ,clipCutHigh_(2000)
    ,wiggleRoom_(100)
    ,negPolarity_(false)
    ,peakFitScaleThreshold_(75)
    // ,pedestal()
    ,peakCutoff_(-1625)
    ,residualCutoff_( 30 )
    ,ratioCutoff_( 0.025)
    ,scaleCutoff_( 30)
    ,artificialDeadTime_(100)
{
    if(preload_splines) LoadSplines();
}

void WaveformFitter::LoadSplines()
{
    std::string splinename;
    std::cout << "Loading splines from: " << spline_file << std::endl;
    TFile *fin = new TFile(spline_file.c_str());
    for (auto &channel : channels)
    {
        splinename = Form("spline_%i_%i", std::get<1>(channel), std::get<2>(channel));
        std::cout << "reading spline: " << splinename << std::endl;
        splines[channel] = (TSpline3 *)(fin->Get(splinename.c_str())->Clone(splinename.c_str()));
        fitters[channel] = fitter::TemplateFitter(*splines[channel], 0, 0);
    }
    fin->Close();
}

void WaveformFitter::LoadConfig(WaveformFitterConfig config)
{
    clipCutLow_ = config.clipCutLow_;
    clipCutHigh_ = config.clipCutHigh_;
    wiggleRoom_ = config.wiggleRoom_;
    negPolarity_ = config.negPolarity_;
    peakFitScaleThreshold_ = config.peakFitScaleThreshold_;
    pedestal = config.pedestal;
    peakCutoff_ = config.peakCutoff_;
    residualCutoff_ = config.residualCutoff_;
    ratioCutoff_ = config.ratioCutoff_;
    scaleCutoff_ = config.scaleCutoff_;
    artificialDeadTime_ = config.artificialDeadTime_;
}

void WaveformFitter::PrintConfig()
{
    std::cout << "Current fitter config" << std::endl;
    std::cout << "    -> clipCutLow_ = " << clipCutLow_ << std::endl;
    std::cout << "    -> clipCutHigh_ = " << clipCutHigh_ << std::endl;
    std::cout << "    -> wiggleRoom_ = " << wiggleRoom_ << std::endl;
    std::cout << "    -> negPolarity_ = " << negPolarity_ << std::endl;
    std::cout << "    -> peakFitScaleThreshold_ = " << peakFitScaleThreshold_ << std::endl;
    std::cout << "    -> pedestal = " << pedestal << std::endl;
    std::cout << "    -> peakCutoff_ = " << peakCutoff_ << std::endl;
    std::cout << "    -> residualCutoff_ = " << residualCutoff_ << std::endl;
    std::cout << "    -> ratioCutoff_ = " << ratioCutoff_ << std::endl;
    std::cout << "    -> scaleCutoff_ = " << scaleCutoff_ << std::endl;
    std::cout << "    -> artificialDeadTime_ = " << artificialDeadTime_ << std::endl;
}

template <typename T>
int WaveformFitter::GetMaxIndex(T trace)
{
    return std::distance(std::begin(trace), std::max_element(std::begin(trace), std::end(trace)));
}


dataProducts::WaveformFit WaveformFitter::Fit(Waveform *w)
{
    // std::cout << "Fitting waveform! "
    //     << w->crateNum << " / "
    //     << w->amcNum << " / "
    //     << w->channelTag << " / "
    // << std::endl;
    dataProducts::WaveformFit output = dataProducts::WaveformFit(w);
    auto this_spline = splines[w->GetID()];
    auto tf = fitters[w->GetID()];
    output.spline = this_spline;

    // begin copied fit logic from TemplateFitApplier.cxx
    // Aaron remains a genius.

    //
    // copy the trace into a local variable we can modify
    // eliminate clipped samples, and save sample nums
    //
    std::vector<short> fitTrace;
    std::vector<unsigned int> sampleNums, underflowSampleNums, overflowSampleNums;
    fitTrace.reserve(w->trace.size());
    sampleNums.reserve(w->trace.size());

    bool clipping = false;
    for (unsigned int i = 0; i < w->trace.size(); ++i)
    {
        short sample = w->trace[i];
        if (sample >= getClipCutHigh())
        {
            if (!clipping && !isNegativePolarity())
            {
                clipping = true;
            }
            overflowSampleNums.push_back(i);
        }
        else if (sample <= getClipCutLow())
        {
            if (!clipping && isNegativePolarity())
            {
                clipping = true;
            }
            underflowSampleNums.push_back(i);
        }
        else
        {
            fitTrace.push_back(sample);
            sampleNums.push_back(i);
        }
    }

    if (fitTrace.empty())
    {
        // fitResult.status = CrystalHitArtRecord::invalidIsland;
        return {output};
    }

    int sign = isNegativePolarity() ? -1 : 1;
    if (isNegativePolarity())
    {
        // multiply all samples by -1
        // to remove need for branching in rest of this function
        std::for_each(fitTrace.begin(), fitTrace.end(),
                      [sign](auto &sampl)
                      { sampl *= sign; });
    }

    std::size_t peakIndex = std::distance(
        fitTrace.begin(), std::max_element(fitTrace.begin(), fitTrace.end()));

    // check size of pulse placed at peak sample
    // calling fit with 0 iterations simply solves for scales
    // given initial time guess
    const int originalMaxIterations = tf.getMaxIterations();
    tf.setMaxIterations(0);
    const auto peakTimeFitOut =
        tf.discontiguousFit(fitTrace, sampleNums, sampleNums[peakIndex]);
    tf.setMaxIterations(originalMaxIterations);

    bool underThresh = (peakTimeFitOut.scales[0] < peakFitScaleThreshold_) && ((fitTrace[peakIndex] - sign * pedestal) < sign * peakCutoff_);
    // bool underThresh = (peakTimeFitOut.scales[0] < peakFitScaleThreshold_) ;

    if (underThresh ||
        (peakIndex <= wiggleRoom_) ||
        (peakIndex + wiggleRoom_ >= fitTrace.size()))
    {
        // std::cout << "under threshold/too close to edge " 
        //     << peakTimeFitOut.scales[0] << " vs. " <<  peakFitScaleThreshold_ 
        //     << " | peakCutoff " << (fitTrace[peakIndex] - sign * pedestal) << " / " << sign * peakCutoff_
        //     << " | peak " << peakIndex
        //     << " -> no fits" << std::endl;
    //     // don't try to fit if amplitude is too small or
    //     // peak is too close to the edge of the island
    //     // fitResult.chi2 = 0;
    //     // fitResult.status = CrystalHitArtRecord::underThreshold;
    //     // fitResult.energy = 0;
    //     // fitResult.time = getTimeOffset(traceRecord) + sampleNums[peakIndex]; // an indicator of where this pulse is located
    //     // fitResult.pedestal = 0;

        return output;
    }
    // std::cout << "passed threshold cut: " << peakTimeFitOut.times[0] << " / " << peakTimeFitOut.scales[0] << " -> " << peakTimeFitOut.chi2 << std::endl;

    // over threshold, so do the fit.
    //
    // first, remove samples from fitTrace so it is the configured length
    //

    // put initial fit guess time at the peakIndex if not clipping,
    // else in the center of the overflow/underflow region
    double peakTime = sampleNums[peakIndex];
    if (clipping)
    {
        const auto &peakClippingSamples =
            isNegativePolarity() ? underflowSampleNums : overflowSampleNums;
        if (peakClippingSamples.size())
        {
            peakTime = peakClippingSamples[peakClippingSamples.size() / 2];
        }
    }

    //
    // BEGIN FITTING LOGIC
    //

    // we will try three times at three different times
    // if it doesn't succeed by then, we call it a failed fit.
    fitter::TemplateFitter::Output singlePulseOut;

    // try extra peak time guesses if pulse is clipping
    const std::vector<int> timeOffsets =
        clipping ? std::vector<int>{0, 1, -1, 2, -2} : std::vector<int>{0, 1, -1};

    bool successfulSingleFit = false;
    for (std::size_t i = 0; (!successfulSingleFit) && (i < timeOffsets.size());
         ++i)
    {
        // for now noise is set to one here,
        // gives same time and energy as long as noise is flat
        singlePulseOut =
            tf.discontiguousFit(fitTrace, sampleNums, peakTime + timeOffsets[i]);

        // allow some extra wiggle room for clipped pulses
        const auto allowedDrift = clipping ? 2 * wiggleRoom_ : wiggleRoom_;

        // std::cout << "   -> Intermediate fit result " << i << " (success, " << successfulSingleFit << "): " << singlePulseOut.times[0] << " / " << singlePulseOut.scales[0] << " -> " << singlePulseOut.chi2 << std::endl;
        if ((std::abs(singlePulseOut.times[0] - peakTime) < allowedDrift) &&
            (singlePulseOut.converged) &&
            (singlePulseOut.scales[0] > scaleCutoff_))
        {
            successfulSingleFit = true;
            // std::cout << "       -> success!" << std::endl;
        }
    }

    // Addition to Logic, JL 2023
    // If this process made the chi2 worse, get the original result

    // prepare to fit additional pulses
    auto multiPulseOut = singlePulseOut;

    // if single pulse time has been pulled too far from peak time,
    // use peak time as initial guess for multi pulse fitting instead
    // don't try this procedure if clipping, though
    // peakTime could be quite off from true pulse time in that case
    if (!clipping && std::abs(singlePulseOut.times[0] - peakTime) > 1)
    {
        multiPulseOut = peakTimeFitOut;
        // std::cout << "Moving back to peak fit!" << std::endl;
    }

    // update peakTime with our current best guess:
    // old peakTime if questionable single pulse fit,
    // singlePulseFit time otherwise
    peakTime = multiPulseOut.times[0];

    // try to fit additional pulses until a fit fails
    // or no residuals are over the cutoff
    bool keepTrying = true;
    while (keepTrying)
    {
        const auto residuals = tf.getResiduals();

        // find peak in residuals
        // not including those within ADT of the single pulse peak time
        double maxResidual = -1;
        int maxResidualIndex = -1;
        for (unsigned int i = 0; i < residuals.size(); ++i)
        {
            if (std::abs(sampleNums[i] - peakTime) > artificialDeadTime_ &&
                residuals[i] > maxResidual)
            {
                maxResidual = residuals[i];
                maxResidualIndex = i;
            }
        }

        // try another pulse fit?
        if (maxResidualIndex != -1 && maxResidual > residualCutoff_)
        {
            fitter::TemplateFitter::Output anotherPulseOut;
            bool successfulNextFit = false;

            // try at three times again to help with convergence
            for (std::size_t i = 0; (!successfulNextFit) && (i < timeOffsets.size());
                 ++i)
            {
                auto timeGuesses = multiPulseOut.times;
                timeGuesses.push_back(sampleNums[maxResidualIndex] + timeOffsets[i]);

                anotherPulseOut =
                    tf.discontiguousFit(fitTrace, sampleNums, timeGuesses);

                // check for convergence
                if (anotherPulseOut.converged)
                {
                    successfulNextFit = true;

                    // check whether each pulse is in the approximately right place
                    for (std::size_t i = 0; i < anotherPulseOut.times.size(); ++i)
                    {
                        if (std::abs(anotherPulseOut.times[i] - timeGuesses[i]) >=
                            2 * wiggleRoom_)
                        {
                            successfulNextFit = false;
                        }
                    }

                    // check whether any times are out of range
                    for (const auto &time : anotherPulseOut.times)
                    {
                        if ((time < sampleNums[0]) || (time >= sampleNums.back()))
                        {
                            successfulNextFit = false;
                        }
                    }

                    // check whether any delta_ts too small
                    auto sortedTimes = anotherPulseOut.times;
                    std::sort(sortedTimes.begin(), sortedTimes.end());
                    for (std::size_t i = 1; i < sortedTimes.size(); ++i)
                    {
                        if (sortedTimes[i] - sortedTimes[i - 1] < artificialDeadTime_)
                        {
                            successfulNextFit = false;
                        }
                    }

                    // check for any energies that are too small
                    for (const auto &scale : anotherPulseOut.scales)
                    {
                        successfulNextFit &= scale > scaleCutoff_;
                    }

                    // check whether ratio to largest pulse too small
                    for (const auto &scale : anotherPulseOut.scales)
                    {
                        successfulNextFit &=
                            scale / anotherPulseOut.scales[0] > ratioCutoff_;
                    }
                } // end if (anotherPulseOut.converged)
                // std::cout << "   -> Intermediate multi-fit (" << anotherPulseOut.times.size() << ") result " << i << " (success, " << successfulNextFit << ") -> " << anotherPulseOut.chi2 << std::endl;
            }     // end fitting loop for new pulse

            if (successfulNextFit)
            {
                multiPulseOut = anotherPulseOut;
            }
            else
            {
                // failed to fit another pulse, give up
                keepTrying = false;
            }
        }
        else
        {
            // no residuals over threshold, no need to keep trying to fit more pulses
            keepTrying = false;
        }
    } // end multipulse fit loop

    // record results
    bool fitterSucceeded;
    fitter::TemplateFitter::Output out;
    if (multiPulseOut.times.size() > 1)
    {
        // this means we successfully fit multiple pulses
        out = multiPulseOut;
        fitterSucceeded = true;
    }
    else
    {
        // we never had a successful multi pulse fit
        out = singlePulseOut;
        fitterSucceeded = successfulSingleFit;
    }
    // std::cout << "Ending with " << out.times.size() << " pulses!" << std::endl;

    // copy the output
    output.times = out.times;
    output.chi2 = out.chi2;
    output.converged = out.converged;
    output.amplitudes = out.scales;
    output.pedestalLevel = out.pedestal;
    output.nfit = out.times.size();

    return output;
}

void WaveformFitter::WriteSplines(TFile *f)
{
    std::string name = "splines";
    f->mkdir(name.c_str());
    f->cd(name.c_str());
    for (auto &map_object : splines)
    {
        std::cout << "Writing spline: " << map_object.second->GetName() << std::endl;
        map_object.second->Write(map_object.second->GetName());
    }
    f->cd("..");
}

WaveformFitter::~WaveformFitter() {}

void WaveformFitter::Show() const
{
    std::ostringstream oss;
    oss << std::endl;
    oss << "WaveformFitter:" << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}