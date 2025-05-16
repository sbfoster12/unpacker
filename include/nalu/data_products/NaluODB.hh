#ifndef NALUODB_HH 
#define NALUODB_HH

#include "common/data_products/DataProduct.hh"

namespace dataProducts {

    class NaluODB : public DataProduct {

        public:
            NaluODB();

            NaluODB(std::string NaluODBString);

            //Destructor
            ~NaluODB();

            //odb string
            std::string odb_string;

            void Show() const override;

            ClassDefOverride(NaluODB,1)

    };

    typedef std::vector<NaluODB> NaluODBCollection;

}

#endif // NALUODB_HH