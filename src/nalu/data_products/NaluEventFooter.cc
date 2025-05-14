#include "nalu/data_products/NaluEventFooter.hh"

using namespace dataProducts;

/**
 * @brief Default constructor for NaluEventFooter.
 *
 * Initializes the event footer to 0.
 */
NaluEventFooter::NaluEventFooter()
    : DataProduct(),
      event_footer(0)
{}

/**
 * @brief Constructor with footer value.
 *
 * @param event_footer_arg 32-bit unsigned integer representing the event footer value.
 */
NaluEventFooter::NaluEventFooter(
    uint32_t event_footer_arg
    )
    : DataProduct(),
      event_footer(event_footer_arg)
{}

/**
 * @brief Destructor for NaluEventFooter.
 */
NaluEventFooter::~NaluEventFooter() {}

/**
 * @brief Make a string of the contents of the class
 *
 * Displays the event footer value in hexadecimal format.
 */
std::string NaluEventFooter::String() const {
    std::ostringstream oss;
    oss << std::endl;
    oss << "NaluEventFooter: " << std::endl;
    oss << "    event_footer: " << std::hex << "0x" << event_footer << std::endl;
    return oss.str();
}

/**
 * @brief Print the contents of the NaluEventFooter to standard output.
 *
 * Displays the class as a string
 */
void NaluEventFooter::Show() const { 
    std::cout << this->String();
}
