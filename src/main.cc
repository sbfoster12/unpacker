/*

Some description...

*/

// Standard
#include <iostream>
#include <memory>
#include <fstream>
#include <time.h>
#include <filesystem>

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TBufferJSON.h>

// Custom - Common
#include "common/unpacking/UnpackerHelpers.hh"
#include "common/unpacking/Logger.hh"

// Custom - Nalu
#include "nalu/unpacking/NaluEventUnpacker.hh"
#include "nalu/data_products/NaluEventHeader.hh"
#include "nalu/data_products/NaluPacketHeader.hh"
#include "nalu/data_products/NaluWaveform.hh"
#include "nalu/data_products/NaluPacketFooter.hh"
#include "nalu/data_products/NaluEventFooter.hh"
#include "nalu/data_products/NaluTime.hh"

#include <string>
#include <sstream>

int main(int argc, char *argv[])
{

    // -----------------------------------------------------------------------------------------------
    // Parse command line arguments

    int verbosity = 2;

    // We need three arguments: program & file name
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [input_file_name]" << std::endl;
        return 1;
    }

    // input file name
    std::string input_file_name = argv[1];

    // Check if input file exists
    if (!std::filesystem::exists(input_file_name)) {
        std::cerr << "Input file doesn't exist. Could not find " << input_file_name << std::endl;
        return 1;
    }

    // Set verbosity for unpacker
    utils::LoggerHolder::getInstance().SetVerbosity(verbosity);

    // output file name
    std::string output_file_name;
    output_file_name = input_file_name.substr(input_file_name.find_last_of("/\\") + 1);
    output_file_name = output_file_name.substr(0, output_file_name.find_last_of('.')) + ".root";
    utils::LoggerHolder::getInstance().InfoLogger << "Output file: " << output_file_name << std::endl;

    // End of parsing command line arguments
    // -----------------------------------------------------------------------------------------------

    // create the output file structure
    TFile *outfile = new TFile(output_file_name.c_str(),"RECREATE");

    // without these next lines, time to process 1 file on my local machine: 5.500s, 62MB
    // outfile->SetCompressionLevel(0); // much faster, but the file size doubles (62->137 MB), 2.936s
    outfile->SetCompressionAlgorithm(4); // LZ4. 40-50% faster, but slightly larger file sizes. 3.292s, 91MB
    TTree *tree = new TTree("tree", "tree");
    // TTree *treeSimple = new TTree("treeSimple", "treeSimple");

    dataProducts::NaluEventHeaderCollection nalu_event_headers;
    tree->Branch("nalu_event_headers", &nalu_event_headers);

    dataProducts::NaluPacketHeaderCollection nalu_packet_headers;
    tree->Branch("nalu_packet_headers", &nalu_packet_headers);

    dataProducts::NaluWaveformCollection nalu_waveforms;
    tree->Branch("nalu_waveforms", &nalu_waveforms);

    dataProducts::NaluPacketFooterCollection nalu_packet_footers;
    tree->Branch("nalu_packet_footers", &nalu_packet_footers);

    dataProducts::NaluEventFooterCollection nalu_event_footers;
    tree->Branch("nalu_event_footers", &nalu_event_footers);

    dataProducts::NaluTimeCollection nalu_times;
    tree->Branch("nalu_times", &nalu_times);

    // std::vector<short> trace;
    // int channel;
    // uint64_t timeStamp;
    // treeSimple->Branch("trace", &trace);
    // treeSimple->Branch("channel", &channel);
    // treeSimple->Branch("timeStamp", &timeStamp);
    // allow for tref usage in t->Draw() commands
    // this might slow things down a bit...
    tree->BranchRef();
    // -----------------------------------------------------------------------------------------------

    // Set up an event unpacker object
    // This object contains the methods for
    // doing the unpacking of a TMEvent
    auto eventUnpacker = new unpackers::NaluEventUnpacker();

    // We need to get a midas event
    TMReaderInterface *mReader = TMNewReader(input_file_name.c_str());

    int nTotalMidasEvents = 0;
    int nSkippedMidasEvents= 0;

    // loop over the events
    while (true)
    {
        TMEvent *thisEvent = TMReadEvent(mReader);
        //if (!thisEvent || nTotalMidasEvents > 100 )
        if (!thisEvent || nTotalMidasEvents > 100 )
        {
            // Reached end of the file. Clean up and break
            delete thisEvent;
            break;
        }

        if (thisEvent->serial_number % 100 == 0) {
            utils::LoggerHolder::getInstance().InfoLogger << "event_id: " << thisEvent->event_id << ", serial number: " << thisEvent->serial_number << std::endl;
        }
        
        int event_id = thisEvent->event_id;

        // Skip event if it is an internal midas event
        if (unpackers::IsHeaderEvent(thisEvent)) {
            delete thisEvent;
            continue;
        }

        thisEvent->FindAllBanks();
        // thisEvent->PrintBanks();
        // auto bank = thisEvent->FindBank("AD%0");
        // std::cout << thisEvent->BankToString(bank) << std::endl;

        // only unpack events with id 1
        if (event_id > 0) {
            nTotalMidasEvents++;
            utils::LoggerHolder::getInstance().InfoLogger << "Processing event " << nTotalMidasEvents << std::endl;
            // Unpack the event
            // This will fill the dataproduct collections
            auto status = eventUnpacker->UnpackEvent(thisEvent);
        
            // std::cout << "status: " << status << std::endl;
            // Only proceed if the status = 0
            if (status != 0) {
                delete thisEvent;
                continue;
            }
            // break;

            nalu_event_headers = eventUnpacker->GetCollection<dataProducts::NaluEventHeader>("NaluEventHeaderCollection");
            nalu_packet_headers = eventUnpacker->GetCollection<dataProducts::NaluPacketHeader>("NaluPacketHeaderCollection");
            nalu_waveforms = eventUnpacker->GetCollection<dataProducts::NaluWaveform>("NaluWaveformCollection");
            nalu_packet_footers = eventUnpacker->GetCollection<dataProducts::NaluPacketFooter>("NaluPacketFooterCollection");
            nalu_event_footers = eventUnpacker->GetCollection<dataProducts::NaluEventFooter>("NaluEventFooterCollection");
            nalu_times = eventUnpacker->GetCollection<dataProducts::NaluTime>("NaluTimeCollection");

            tree->Fill();
            nalu_event_headers.clear();


            // // Get a vector of waveform collections
            // auto waveformsVector = eventUnpacker->GetCollectionVector<dataProducts::Waveform>("WaveformCollection", &dataProducts::Waveform::waveformIndex);

            // std::cout << "waveformsVector.size(): " << waveformsVector.size() << std::endl;
            // //Check if each there are any waveforms
            // if (waveformsVector.size() == 0) {
            //     std::cout << "Warning: skipped a midas event because it had no waveforms." << std::endl;
            //     nSkippedMidasEvents++;
            //     continue;
            // }

//            //Check if each waveform index has the same number of events
//            bool skipMidasEvent = false;
//            int number_of_waveforms = waveformsVector.front().size(); //check above ensures there is at least one entry
//            std::cout << "number_of_waveforms: " << number_of_waveforms << std::endl;
//            for (auto &wfs : waveformsVector) {
//                std::cout << "  wfs.size: " << wfs.size() << std::endl;
//                if (wfs.size() != (uint)number_of_waveforms) {
//                    skipMidasEvent = true;
//                    std::cout << " waveformsVector.front().size(): " <<  waveformsVector.front().size() << std::endl;
//                    break;
//                }
//            }
//            if (skipMidasEvent) {
//                std::cout << "Warning: skipped  midas event " << thisEvent->serial_number << " because number of waveforms doesn't match between AMC slots." << std::endl;
//                nSkippedMidasEvents++;
//                continue;
//            }

            // std::vector<uint64_t> timestamps[5];
            // waveforms.clear();
            // timestamps[0].clear();
            // timestamps[1].clear();
            // timestamps[2].clear();
            // timestamps[3].clear();
            // timestamps[4].clear();
            // for (auto &waveforms_tmp : waveformsVector) {
            //     waveforms = waveforms_tmp;
            //     trace.clear();
            //     for ( auto &waveform_loc : waveforms ) {
            //         trace = waveform_loc.trace;
            //         channel = waveform_loc.channelTag;
            //         timeStamp = waveform_loc.clockCounter;
            //         timestamps[channel].push_back(timeStamp);
            //         treeSimple->Fill();
            //         trace.clear();
            //     }

        // tree->Fill();
        // waveforms.clear();
                
            // }
//          for ( auto iTime = 0; iTime < timestamps[0].size(); ++iTime ) {
//            std::cout << "i, dt's: " << iTime << " "
//                                     << (long)timestamps[1][iTime] - (long)timestamps[0][iTime] << " "
//                                     << (long)timestamps[2][iTime] - (long)timestamps[0][iTime] << " "
//                                     << (long)timestamps[3][iTime] - (long)timestamps[0][iTime] << " "
//                                     << (long)timestamps[4][iTime] - (long)timestamps[0][iTime] << std::endl;
//          }
        } // end if event id = 1

    } // end loop over events

    // clean up
    delete eventUnpacker;
    delete mReader;

    tree->Write();
    // treeSimple->Write();
    outfile->Close();

    utils::LoggerHolder::getInstance().InfoLogger << "Skipped " << nSkippedMidasEvents << "/" << nTotalMidasEvents << " midas events" << std::endl;

    utils::LoggerHolder::getInstance().InfoLogger << "All done!" << std::endl;
    return 0;
}
