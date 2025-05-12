#ifndef ODB_HH 
#define ODB_HH

#include "DataProduct.hh"
#include "ODBParser.hh"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace dataProducts {

    class ODB : public DataProduct {

        public:
            ODB();

            ODB(std::string odbString);

            //Destructor
            ~ODB();

            void ParseODB();

            //xlm odb dump
            std::string odbString;

            //run info
            int runNumber;
            int subRunNumber;

            //waveform digitization frequencies, presamples (amc slot number -> digitization frequency in MHz)
            std::map<int,int> digitizationFrequencies; 
            std::map<int,float> waveformPresamples; 

            //stage position
            int xStage;
            int yStage;

            void Show() const override;

            ClassDefOverride(ODB,3)

    };

    typedef std::vector<ODB> ODBCollection;

}

#endif // ODB_HH