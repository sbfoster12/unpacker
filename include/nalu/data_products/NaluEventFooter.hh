#ifndef NALUEVENTFOOTER_HH   
#define NALUEVENTFOOTER_HH

#include "common/data_products/DataProduct.hh"

namespace dataProducts {

    /**
     * @class NaluEventFooter
     * @brief Represents the footer information for a NALU event.
     *
     * Inherits from DataProduct. 
     */
    class NaluEventFooter : public DataProduct {

        public:
            /**
             * @brief Default constructor.
             */
            NaluEventFooter();

            /**
             * @brief Constructor with event_footer index.
             * @param event_footer 2 byte word in the footer
             */
            NaluEventFooter(uint32_t event_footer);

            /**
             * @brief Destructor.
             */
            ~NaluEventFooter();

            /**
             * @brief Index of the event.
             */
            unsigned int event_footer;

            /**
             * @brief Make a string of this class's contents.
             */
            std::string String() const;

            /**
             * @brief Display the contents of the NaluEventFooter.
             */
            void Show() const override;

            /// ROOT class definition macro with versioning
            ClassDefOverride(NaluEventFooter,1)
    };

    /**
     * @typedef NaluEventFooterCollection
     * @brief A collection (vector) of NaluEventFooter objects.
     */
    typedef std::vector<NaluEventFooter> NaluEventFooterCollection;

} // namespace dataProducts

#endif // NALUEVENTFOOTER_HH
