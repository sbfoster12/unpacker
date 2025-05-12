#ifndef HISTOGRAMMAKER_HH 
#define HISTOGRAMMAKER_HH

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraph.h"
#include "TTree.h"
#include "TFile.h"
#include "TBuffer.h"
#include "TMath.h"

#include "DataProduct.hh"
// #include "serializer/Serializer.hh"

namespace dataProducts {


    class HistogramMaker : public dataProducts::DataProduct {

        public:
            HistogramMaker();
            HistogramMaker(TTree *t, ChannelList s);
            HistogramMaker(ChannelList s);

            //Destructor
            ~HistogramMaker();

            bool write_empty_histograms;
            void SetWriteEmpty(bool e) { write_empty_histograms = e;};

            void MakeStructure(ChannelList s);
            void Fill(TTree *t, ChannelList s);
            // std::vector<TH1I*> energy_histograms_uncalibrated_fullintegral;
            // std::vector<TH1I*> energy_histograms_uncalibrated_integral;
            
            // energy integral histograms by channel
            std::map<ChannelID, TH1I*> energy_histograms_uncalibrated_fullintegral;
            std::map<ChannelID, TH1I*> energy_histograms_uncalibrated_integral;
            std::map<ChannelID, TH2I*> energy_histograms_uncalibrated_integral_rf;
            std::map<ChannelID, TH1I*> energy_histograms_calibrated_integral;

            // position histograms, hodoscope and xtal array
            TH2F * hodoscope_hit_positions;
            TH2F * hodoscope_max_hit_channel_positions;
            TH2F * hodoscope_hit_positions_calibrated;
            TH2F * hodoscope_max_hit_channel_positions_calibrated;
            TH3F * calo_hit_positions_by_energy;


            void WriteAllHistograms(TFile* f);
            template <typename T> void WriteHistogramVec(TDirectory* f, std::string name, T vec);
            template <typename T> void WriteHistogram(TDirectory* f, std::string name, T h);

            void Show() const override;

            ClassDefOverride(HistogramMaker,3)

    };

}

#endif // HISTOGRAMMAKER_HH