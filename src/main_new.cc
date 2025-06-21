/*
Some description...
*/

// Standard
#include <iostream>
#include <memory>
#include <fstream>
#include <time.h>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TBufferJSON.h>
#include <TSystem.h>
#include <TClass.h>
#include <TDataMember.h>

// Custom - Common
#include "common/unpacking/Logger.hh"

// Custom - Nalu
#include "nalu/unpacking/NaluEventUnpacker.hh"
#include "nalu/data_products/NaluEventHeader.hh"
#include "nalu/data_products/NaluPacketHeader.hh"
#include "nalu/data_products/NaluWaveform.hh"
#include "nalu/data_products/NaluPacketFooter.hh"
#include "nalu/data_products/NaluEventFooter.hh"
#include "nalu/data_products/NaluTime.hh"
#include "nalu/data_products/NaluODB.hh"


// Helper function to print ROOT class reflection info
void PrintRootClassInfo(const char* className)
{
    TClass* cls = TClass::GetClass(className);
    if (!cls) {
        std::cout << "Class " << className << " not found by ROOT dictionary!" << std::endl;
        return;
    }

    std::cout << "Class: " << cls->GetName() << std::endl;
    std::cout << "Title: " << cls->GetTitle() << std::endl;
    std::cout << "Number of Data Members: " << cls->GetListOfDataMembers()->GetEntries() << std::endl;

    TIter next(cls->GetListOfDataMembers());
    TDataMember* dm;
    while ((dm = (TDataMember*)next())) {
        std::cout << " - " << dm->GetName() << " (" << dm->GetTypeName() << ")" << std::endl;
    }
    std::cout << std::endl;
}


// Helper function to print ROOT class members and their values (basic types only)
void PrintMembersFromBase(const dataProducts::DataProduct* obj) {
    if (!obj) {
        std::cout << "[PrintMembersFromBase] Null pointer received." << std::endl;
        return;
    }

    TClass* actualClass = TClass::GetClass(typeid(*obj));
    if (!actualClass) {
        std::cout << "[PrintMembersFromBase] Could not determine class for object." << std::endl;
        return;
    }

    std::cout << "\n== Runtime type: " << actualClass->GetName() << " ==" << std::endl;

    const TCollection* membersList = cls->GetListOfDataMembers();
    TIter next(membersList);
    TDataMember* dm;
    while ((dm = (TDataMember*)next())) {
        Long_t offset = dm->GetOffset();
        const char* memberName = dm->GetName();
        const char* typeName = dm->GetTypeName();
        void* memberPtr = (char*)obj + offset;

        std::cout << " - " << memberName << " (" << typeName << ") = ";

        if (strcmp(typeName, "int") == 0) {
            std::cout << *(int*)memberPtr;
        } else if (strcmp(typeName, "unsigned int") == 0) {
            std::cout << *(unsigned int*)memberPtr;
        } else if (strcmp(typeName, "unsigned short") == 0) {
            std::cout << *(unsigned short*)memberPtr;
        } else if (strcmp(typeName, "unsigned long") == 0) {
            std::cout << *(unsigned long*)memberPtr;
        } else if (strcmp(typeName, "double") == 0) {
            std::cout << *(double*)memberPtr;
        } else if (strcmp(typeName, "std::string") == 0 || strcmp(typeName, "string") == 0) {
            std::cout << *(std::string*)memberPtr;
        } else if (strstr(typeName, "vector") != nullptr) {
            std::cout << "[vector]";
        } else {
            std::cout << "<unsupported type>";
        }

        std::cout << std::endl;
    }
}


int main(int argc, char *argv[])
{
    // -----------------------------------------------------------------------------------------------
    // Parse command line arguments

    // We need three arguments: program & file name & verbosity
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " input_file_name verbosity" << std::endl;
        return 1;
    }

    // input file name
    std::string input_file_name = argv[1];

    int verbosity = std::atoi(argv[2]);

    // Check if input file exists
    if (!std::filesystem::exists(input_file_name)) {
        std::cerr << "Input file doesn't exist. Could not find " << input_file_name << std::endl;
        return 1;
    }

    // output file name
    std::string output_file_name;
    output_file_name = input_file_name.substr(input_file_name.find_last_of("/\\") + 1);
    output_file_name = output_file_name.substr(0, output_file_name.find_last_of('.')) + ".root";
    std::cout << "Output file: " << output_file_name << std::endl;

    // Set verbosity for unpacker
    utils::LoggerHolder::getInstance().SetVerbosity(verbosity);

    // End of parsing command line arguments
    // -----------------------------------------------------------------------------------------------

    // create the output file structure
    TFile *outfile = new TFile(output_file_name.c_str(),"RECREATE");

    // Compression settings
    // outfile->SetCompressionLevel(0); // no compression, faster but bigger
    outfile->SetCompressionAlgorithm(4); // LZ4 compression: faster & decent size

    TTree *tree = new TTree("tree", "tree");

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

    dataProducts::NaluODB odb;

    // -----------------------------------------------------------------------------------------------

    // Set up an event unpacker object
    auto eventUnpacker = new unpackers::NaluEventUnpacker();

    // Create MIDAS reader
    TMReaderInterface *mReader = TMNewReader(input_file_name.c_str());

    int nTotalMidasEvents = 0;
    int nSkippedMidasEvents = 0;

    // Loop over the events
    while (true)
    {
        TMEvent *thisEvent = TMReadEvent(mReader);
        if (!thisEvent || nTotalMidasEvents > 100)
        {
            // Reached end of file or limit reached
            delete thisEvent;
            break;
        }

        if (thisEvent->serial_number % 100 == 0) {
            std::cout << "event_id: " << thisEvent->event_id << ", serial number: " << thisEvent->serial_number << std::endl;
        }

        int event_id = thisEvent->event_id;

        // Check internal MIDAS event
        if (unpackers::IsHeaderEvent(thisEvent)) {
            // Check if BOR event
            if (event_id == 32768) {
                std::vector<char> data = thisEvent->data;
                std::string odb_dump(data.begin(), data.end());
                std::size_t pos = odb_dump.find('{');
                if (pos != std::string::npos) {
                    odb_dump.erase(0, pos);  // Keep from first '{'
                }
                odb = dataProducts::NaluODB(odb_dump);
                outfile->WriteObject(&odb, "nalu_odb");
            }
            delete thisEvent;
            continue;
        }

        thisEvent->FindAllBanks();

        if (event_id > 0) {
            nTotalMidasEvents++;

            int status = eventUnpacker->UnpackEvent(thisEvent);

            if (status != 0) {
                delete thisEvent;
                nSkippedMidasEvents++;
                continue;
            }

            nalu_event_headers = eventUnpacker->GetCollection<dataProducts::NaluEventHeader>("NaluEventHeaderCollection");
            nalu_packet_headers = eventUnpacker->GetCollection<dataProducts::NaluPacketHeader>("NaluPacketHeaderCollection");
            nalu_waveforms = eventUnpacker->GetCollection<dataProducts::NaluWaveform>("NaluWaveformCollection");
            nalu_packet_footers = eventUnpacker->GetCollection<dataProducts::NaluPacketFooter>("NaluPacketFooterCollection");
            nalu_event_footers = eventUnpacker->GetCollection<dataProducts::NaluEventFooter>("NaluEventFooterCollection");
            nalu_times = eventUnpacker->GetCollection<dataProducts::NaluTime>("NaluTimeCollection");

            if (nTotalMidasEvents <= 1) {
                std::cout << "\n=== Example print of first object members from each collection ===" << std::endl;

                if (!nalu_event_headers.empty())
                    PrintMembersFromBase(&nalu_event_headers[0]);
                if (!nalu_packet_headers.empty())
                    PrintMembersFromBase(&nalu_packet_headers[0]);
                if (!nalu_waveforms.empty())
                    PrintMembersFromBase(&nalu_waveforms[0]);
                if (!nalu_packet_footers.empty())
                    PrintMembersFromBase(&nalu_packet_footers[0]);
                if (!nalu_event_footers.empty())
                    PrintMembersFromBase(&nalu_event_footers[0]);
                if (!nalu_times.empty())
                    PrintMembersFromBase(&nalu_times[0]);
            }

            tree->Fill();

            nalu_event_headers.clear();
            nalu_packet_headers.clear();
            nalu_waveforms.clear();
            nalu_packet_footers.clear();
            nalu_event_footers.clear();
            nalu_times.clear();
        }

        delete thisEvent;
    } // end loop over events

    // Write tree and close output file
    tree->Write();
    outfile->Close();

    // Print ROOT reflection info for data products
    std::cout << "\n=== ROOT Reflection Info for Data Products ===\n";
    PrintRootClassInfo("dataProducts::NaluEventHeader");
    PrintRootClassInfo("dataProducts::NaluPacketHeader");
    PrintRootClassInfo("dataProducts::NaluWaveform");
    PrintRootClassInfo("dataProducts::NaluPacketFooter");
    PrintRootClassInfo("dataProducts::NaluEventFooter");
    PrintRootClassInfo("dataProducts::NaluTime");
    PrintRootClassInfo("dataProducts::NaluODB");


    // Cleanup
    delete eventUnpacker;
    delete mReader;

    std::cout << "Skipped " << nSkippedMidasEvents << "/" << nTotalMidasEvents << " midas events" << std::endl;
    std::cout << "All done!" << std::endl;



    // Test creating a new NaluEventUnpacker instance dynamically via ROOT dictionary
    TClass* unpackerClass = TClass::GetClass("unpackers::NaluEventUnpacker");
    if (!unpackerClass) {
        std::cerr << "ERROR: ROOT dictionary for unpackers::NaluEventUnpacker not found!" << std::endl;
    } else {
        // Add explicit cast from void* to TObject*
        TObject* obj = static_cast<TObject*>(unpackerClass->New());
        if (!obj) {
            std::cerr << "ERROR: Failed to instantiate unpackers::NaluEventUnpacker via ROOT!" << std::endl;
        } else {
            std::cout << "Successfully created a new unpackers::NaluEventUnpacker instance via ROOT!" << std::endl;
            
            // You can cast to proper type if you want to use it normally
            unpackers::NaluEventUnpacker* unpackerPtr = dynamic_cast<unpackers::NaluEventUnpacker*>(obj);
            if (unpackerPtr) {
                // Use unpackerPtr as usual here, e.g., call methods, etc.
                std::cout << "Unpacker instance ready to use." << std::endl;
            } else {
                std::cerr << "ERROR: dynamic_cast failed on the created object." << std::endl;
            }
            
            delete obj; // Clean up
        }
    }


    return 0;
}
