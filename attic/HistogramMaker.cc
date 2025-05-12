#include "HistogramMaker.hh"

using namespace dataProducts;

HistogramMaker::HistogramMaker()
    : DataProduct()
{}

HistogramMaker::HistogramMaker(TTree *t, ChannelList s)
    : DataProduct()
    , write_empty_histograms(false)
{
    Fill(t,s);
}

HistogramMaker::HistogramMaker(ChannelList s)
    : DataProduct()
    , write_empty_histograms(false)
{
    MakeStructure(s);
}

void HistogramMaker::MakeStructure(ChannelList s)
{
    // Create all of the histograms from the channel list. 
    // Uses the serializer to get the mapping of AMC channels
    energy_histograms_uncalibrated_fullintegral.clear();
    energy_histograms_uncalibrated_integral.clear();
    energy_histograms_uncalibrated_integral_rf.clear();
    energy_histograms_calibrated_integral.clear();

    TString histname;
    TString histtitle;
    TString cutstring;
    for ( const auto &map_entry : s ) {
        // cutstring = Form("(integrals.crateNum == %i) && (integrals.amcNum == %i) && (integrals.channelTag == %i)", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        // std::cout << "Getting map entries: " << std::get<0>(map_entry) << " / " << std::get<1>(map_entry) << " / " << std::get<2>(map_entry) << "\n";
        // std::cout << "   -> " << cutstring << std::endl;

        // uncalibrated full integrals
        histname = Form("uncalibrated_full_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        histtitle = Form("Uncalibrated Full Integrals: Crate %i | AMC %i | Channel %i; Pulse Integral [arb. u]", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        energy_histograms_uncalibrated_fullintegral[map_entry] = new TH1I(histname, histtitle, 100, 1000,100000);

        // uncalibrated restricted integrals
        histname = Form("uncalibrated_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        histtitle = Form("Uncalibrated Integrals: Crate %i | AMC %i | Channel %i; Pulse Integral [arb. u]", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        energy_histograms_uncalibrated_integral[map_entry] = new TH1I(histname, histtitle, 100, 1000,100000);

        // uncalibrated restricted integrals vs. rf
        histname = Form("uncalibrated_rf_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        histtitle = Form("Uncalibrated Integrals vs. RF Phase: Crate %i | AMC %i | Channel %i; RF Phase [rad]; Pulse Integral [arb. u]", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        energy_histograms_uncalibrated_integral_rf[map_entry] = new TH2I(histname, histtitle, 100,0,2*TMath::Pi(), 100, 1000,100000 );
        
        // calibrated restricted integrals
        histname = Form("calibrated_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        histtitle = Form("Calibrated Integrals: Crate %i | AMC %i | Channel %i; Pulse Integral [~MeV]", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        energy_histograms_calibrated_integral[map_entry] = new TH1I(histname, histtitle, 100, -1,100);
    } 

    // position histograms
    hodoscope_hit_positions = new TH2F("hodoscope_hit_positions", "Hit Position in Hodoscope; x; y;", 100,-25,25,100,-25,25);
    hodoscope_max_hit_channel_positions = new TH2F("hodoscope_max_hit_channel_positions", "Max Channel Hit Position in Hodoscope; x; y;", 100,-25,25,100,-25,25);
    hodoscope_hit_positions_calibrated = new TH2F("hodoscope_hit_positions_calibrated", "Hit Position in Hodoscope; x; y;", 100,-25,25,100,-25,25);
    hodoscope_max_hit_channel_positions_calibrated = new TH2F("hodoscope_max_hit_channel_positions_calibrated", "Max Channel Hit Position in Hodoscope; x; y;", 100,-25,25,100,-25,25);
    calo_hit_positions_by_energy = new TH3F("calo_hit_positions_by_energy", "Hit Position in Hodoscope; x; y; energy", 100,-5,5,100,-3,3,100,0,200);
}

void HistogramMaker::Fill(TTree *t, ChannelList s)
{
    // **************************************
    // ******DEPRECATED**********************
    // **************************************
    // Create and fill all of the histograms from the trees. 
    // Uses the serializer to get the mapping of AMC channels
    MakeStructure(s);
    // energy_histograms_uncalibrated_fullintegral.clear();
    // energy_histograms_uncalibrated_integral.clear();

    TString histname;
    TString histtitle;
    TString cutstring;
    for ( const auto &map_entry : s ) {
        cutstring = Form("(integrals.crateNum == %i) && (integrals.amcNum == %i) && (integrals.channelTag == %i)", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));

        // uncalibrated full integrals
        histname = Form("uncalibrated_full_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        t->Draw("integrals.fullintegral>>"+histname, cutstring, "goff");

        // uncalibrated restricted integrals
        histname = Form("uncalibrated_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        t->Draw("integrals.integral>>"+histname, cutstring, "goff");

        // uncalibrated vs. rf results
        histname = Form("uncalibrated_rf_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        t->Draw("integrals.integral:rf.corrected_phase>>"+histname, cutstring, "goff");

        // calibrated restricted integrals
        histname = Form("calibrated_%i_%i_%i", std::get<0>(map_entry), std::get<1>(map_entry), std::get<2>(map_entry));
        t->Draw("integrals.integral>>"+histname, cutstring, "goff");
    }
    
}

template <typename T> void HistogramMaker::WriteHistogramVec(TDirectory* f, std::string name, T vec)
{
    // std::cout << "Making histogram vec for name: " << name << std::endl;
    f->mkdir(name.c_str(),"", true);
    f->cd(name.c_str());
    for (auto h: vec)
    {
        if(h.second->GetEntries() > 0 || write_empty_histograms)
            (h.second)->Write("", TObject::kOverwrite);
    }
    f->cd("..");
}

template <typename T> void HistogramMaker::WriteHistogram(TDirectory* f, std::string name, T h)
{
    // std::cout << "Making histogram for name: " << name << std::endl;
    if(h->GetEntries() > 0 || write_empty_histograms)
    {
        f->mkdir(name.c_str(),"", true);
        f->cd(name.c_str());
        h->Write("", TObject::kOverwrite);
        f->cd("..");
    }
}

void HistogramMaker::WriteAllHistograms(TFile *f)
{
    std::string basename = "hists";
    f->cd();
    f->mkdir(basename.c_str(),"",true);
    f->cd(basename.c_str());

    // add additional histogram vectors here
    WriteHistogramVec(f->GetDirectory(basename.c_str()), "uncalibrated_full", energy_histograms_uncalibrated_fullintegral);
    WriteHistogramVec(f->GetDirectory(basename.c_str()), "uncalibrated", energy_histograms_uncalibrated_integral);
    WriteHistogramVec(f->GetDirectory(basename.c_str()), "uncalibrated_rf", energy_histograms_uncalibrated_integral_rf);
    WriteHistogramVec(f->GetDirectory(basename.c_str()), "calibrated", energy_histograms_calibrated_integral);

    WriteHistogram(f->GetDirectory(basename.c_str()), "positions", hodoscope_hit_positions);
    WriteHistogram(f->GetDirectory(basename.c_str()), "positions", hodoscope_max_hit_channel_positions);
    WriteHistogram(f->GetDirectory(basename.c_str()), "positions", hodoscope_hit_positions_calibrated);
    WriteHistogram(f->GetDirectory(basename.c_str()), "positions", hodoscope_max_hit_channel_positions_calibrated);
    WriteHistogram(f->GetDirectory(basename.c_str()), "positions", calo_hit_positions_by_energy);

    std::cout  << "Histograms written to directory '" << basename << "'" << std::endl;
}

HistogramMaker::~HistogramMaker() {}

void HistogramMaker::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "HistogramMaker:" << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}

// template void HistogramMaker::WriteHistogramVec<std::vector<TH1D*>>(TDirectory*, std::string, std::vector<TH1D*>);
// template void HistogramMaker::WriteHistogramVec<std::vector<TH1I*>>(TDirectory*, std::string, std::vector<TH1I*>);
// template void HistogramMaker::WriteHistogramVec<std::vector<TH2D*>>(TDirectory*, std::string, std::vector<TH2D*>);
// template void HistogramMaker::WriteHistogramVec<std::vector<TH2I*>>(TDirectory*, std::string, std::vector<TH2I*>);

// template void HistogramMaker::WriteHistogram<TH1D*>(TDirectory*, std::string, TH1D*);
// template void HistogramMaker::WriteHistogram<TH1I*>(TDirectory*, std::string, TH1I*);
// template void HistogramMaker::WriteHistogram<TH2D*>(TDirectory*, std::string, TH2D*);
// template void HistogramMaker::WriteHistogram<TH2I*>(TDirectory*, std::string, TH2I*);
