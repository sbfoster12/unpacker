#ifndef NALUPACKETHEADER_HH   
#define NALUPACKETHEADER_HH

#include "common/data_products/DataProduct.hh"

namespace dataProducts {

    /**
     * @class NaluPacketHeader
     * @brief Represents the header information for a NALU packet.
     *
     * Inherits from DataProduct.
     */
    class NaluPacketHeader : public DataProduct {

        public:
            /**
             * @brief Default constructor.
             */
            NaluPacketHeader();

            /**
             * @brief Constructor with event index.
             * @param packet_header
             */
            NaluPacketHeader(
                 uint16_t packet_header
                ,uint16_t packet_info
                ,uint16_t channel
                ,uint32_t trigger_time
                ,uint16_t logical_position
                ,uint16_t window_position
            );

            /**
             * @brief Destructor.
             */
            ~NaluPacketHeader();

            /**
             * @brief Packet header.
             */
            uint16_t packet_header;

            /**
             * @brief Packet info
             */
            uint16_t packet_info;

            /**
             * @brief Channel
             */
            uint16_t channel;

            /**
             * @brief Trigger time
             */
            uint32_t trigger_time;

            /**
             * @brief Logical position
             */
            uint16_t logical_position;

            /**
             * @brief Window position
             */
            uint16_t window_position;

            /**
             * @brief Make a string of this class's contents.
             */
            std::string String() const;

            /**
             * @brief Display the contents of the NaluPacketHeader.
             */
            void Show() const override;

            /// ROOT class definition macro with versioning
            ClassDefOverride(NaluPacketHeader,1)
    };

    /**
     * @typedef NaluPacketHeaderCollection
     * @brief A collection (vector) of NaluPacketHeader objects.
     */
    typedef std::vector<NaluPacketHeader> NaluPacketHeaderCollection;

} // namespace dataProducts

#endif // NALUPACKETHEADER_HH
