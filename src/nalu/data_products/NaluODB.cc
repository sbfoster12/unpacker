#include "nalu/data_products/NaluODB.hh"


using namespace dataProducts;

NaluODB::NaluODB()
    : DataProduct()
    ,odb_string("")
{}

NaluODB::NaluODB(std::string odb_string_arg)
    : DataProduct()
    ,odb_string(odb_string_arg)
{}

NaluODB::~NaluODB() {}

void NaluODB::Show() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "NaluODB:" << std::endl;
    oss << odb_string << std::endl;
    oss << std::endl;
    oss << std::endl;
    std::cout << oss.str();
}