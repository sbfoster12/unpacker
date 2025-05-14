#ifndef NALUPACKETFOOTER_HH   
#define NALUPACKETFOOTER_HH

#include "common/data_products/DataProduct.hh"

namespace dataProducts {

    /**
     * @class NaluPacketFooter
     * @brief Represents the footer information for a NALU packet.
     *
     * Inherits from DataProduct.
     */
    class NaluPacketFooter : public DataProduct {

        public:
            /**
             * @brief Default constructor.
             */
            NaluPacketFooter();

            /**
             * @brief Constructor with event index.
             * @param parser_index Parser index
             * @param packet_footer Packet footer
             */
            NaluPacketFooter(uint32_t parser_index, uint32_t packet_footer);

            /**
             * @brief Destructor.
             */
            ~NaluPacketFooter();

            /**
             * @brief Parser index.
             */
            uint32_t parser_index;

            /**
             * @brief Packet footer.
             */
            uint32_t packet_footer;

            /**
             * @brief Make a string of this class's contents.
             */
            std::string String() const;

            /**
             * @brief Display the contents of the NaluPacketFooter.
             */
            void Show() const override;

            /// ROOT class definition macro with versioning
            ClassDefOverride(NaluPacketFooter,1)
    };

    /**
     * @typedef NaluPacketFooterCollection
     * @brief A collection (vector) of NaluPacketFooter objects.
     */
    typedef std::vector<NaluPacketFooter> NaluPacketFooterCollection;

} // namespace dataProducts

#endif // NALUPACKETFOOTER_HH
