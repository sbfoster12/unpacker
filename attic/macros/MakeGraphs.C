#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TSystem.h>


#include "DataProduct.hh"
#include "Waveform.hh"

void MakeGraphs(std::string fileName, int run, int subRun) {

    gSystem->Exec("mkdir -p Images");

    TFile* inputFile = TFile::Open(fileName.c_str(), "READ");
    TFile* outFile = TFile::Open("graphs.root", "RECREATE");
    TDirectory* runDir = outFile->mkdir(Form("Run%d",run)); 
    TDirectory* subRunDir = runDir->mkdir(Form("SubRun%d",subRun)); 

    auto tree = (TTree*)inputFile->Get("tree");
    std::vector<dataProducts::Waveform>* wPtr = nullptr;
    tree->SetBranchAddress("waveforms_raw",&wPtr);

    for (uint iEntry = 0; iEntry < tree->GetEntries(); ++iEntry)
    {
        tree->GetEntry(iEntry);

        TDirectory* eventDir = nullptr;
        if (wPtr->size() > 0) {
            std::cout << "EventNum: " << wPtr->at(0).eventNum << std::endl;
            eventDir = subRunDir->mkdir(Form("Event%d",wPtr->at(0).eventNum));
        } else {
            continue;
        }

        for (uint iWaveform = 0; iWaveform < wPtr->size(); ++iWaveform)
        {
            auto graph = wPtr->at(iWaveform).MakeTGraph();
            auto c1 = wPtr->at(iWaveform).DrawWaveform(graph);
            // c1->SaveAs(Form("Images/Waveform_%d_%d.png",iEntry,iWaveform));
            // c1->SaveAs(Form("Images/Waveform_%d_%d.pdf",iEntry,iWaveform));
            eventDir->cd();
            c1->Write(Form("Waveform_amcNum%d_channelNum%d",wPtr->at(iWaveform).amcNum,wPtr->at(iWaveform).channelTag));

            delete c1;
            delete graph;
        }


        //if (iEntry > 10) break;
    }

   
    inputFile->Close();
    outFile->Close();

}