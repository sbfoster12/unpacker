#ifndef WaveformFitter_HH 
#define WaveformFitter_HH

#include "DataProduct.hh"
#include "Waveform.hh"
#include "WaveformFit.hh"
#include "TSpline.h"

#include "TemplateFitter.hh"

namespace dataProducts {

    struct WaveformFitterConfig {
        short  clipCutLow_;
        short  clipCutHigh_;
        unsigned int wiggleRoom_;
        bool  negPolarity_;
        int peakFitScaleThreshold_;
        int pedestal;
        int peakCutoff_;
        double residualCutoff_;
        double ratioCutoff_;
        double scaleCutoff_;
        double artificialDeadTime_;
    };

    class SingleWaveformFitter : public DataProduct {
        public:
            SingleWaveformFitter();
            ~SingleWaveformFitter();

            SingleWaveformFitter(TSpline3 sp);
            void SetNFit(int n) { nfits = n; };
            Double_t operator()(double *x, double* p)
            {
                // loop over things nfits times
                Double_t output = p[0];
                Double_t xx = x[0];
                for (int i = 0; i < nfits; i++)
                {
                    output += p[1+i*2]*spline.Eval(xx-p[2+i*2]);
                }
                return output;
            };

            ClassDefOverride(SingleWaveformFitter,1)

        private: 
            TSpline3 spline;
            int nfits;
    };

    class WaveformFitter : public DataProduct {

        public:
            WaveformFitter();

            WaveformFitter(std::string spline_file, ChannelList ch, bool preload_splines);

            //Destructor
            ~WaveformFitter();

            // fitter specific
            std::string spline_file;
            std::map< ChannelID, TSpline3* > splines;
            std::map< ChannelID, fitter::TemplateFitter > fitters;
            ChannelList channels;

            void Show() const override;
            void LoadSplines();
            void LoadConfig(WaveformFitterConfig config);
            void PrintConfig();

            // make sure we can write to file
            // void Clean(){delete fitters;};
            void WriteSplines(TFile *f);
            template <typename T> int GetMaxIndex(T trace);

            dataProducts::WaveformFit Fit(Waveform *w);

            // copied from aaron
            
            short getClipCutLow() const { return clipCutLow_; }
            short getClipCutHigh() const { return clipCutHigh_; }
            unsigned int getWiggleRoom() const { return wiggleRoom_; }
            bool isNegativePolarity() const { return negPolarity_; }


            ClassDefOverride(WaveformFitter,1)

        private:
            short  clipCutLow_;
            short  clipCutHigh_;
            unsigned int  wiggleRoom_;
            bool  negPolarity_;
            int peakFitScaleThreshold_;
            int pedestal;
            int peakCutoff_;
            double residualCutoff_;
            double ratioCutoff_;
            double scaleCutoff_;
            double artificialDeadTime_;

    };

    // typedef std::vector<WaveformFitter> WaveformFitterCollection;

}

#endif // WaveformFitter_HH