#ifndef NALUPACKET_HH   
#define NALUPACKET_HH

#include "common/data_products/DataProduct.hh"

namespace dataProducts {

    /**
     * @class NaluPacket
     * @brief Represents the nalu packet for a NALU event.
     *
     * Inherits from DataProduct.
     */
    class NaluPacket : public DataProduct {

        public:
            /**
             * @brief Default constructor.
             */
            NaluPacket();

            /**
             * @brief Constructor with event index.
             * @param event_index Index of the event.
             */
            NaluPacket(
                 uint64_t channel_num
                ,uint64_t window_position
                ,std::vector<short> trace
            );

            /**
             * @brief Destructor.
             */
            ~NaluPacket();

            /**
             * @brief Channel of this packet.
             */
            uint64_t channel_num;

            /**
             * @brief Window position of this packet.
             */
            uint64_t window_position;

            /**
             * @brief Trace associated with this packet.
             */
            std::vector<short> trace;

            /**
             * @brief Display the contents of the NaluPacket.
             */
            void Show() const override;

            /// ROOT class definition macro with versioning
            ClassDefOverride(NaluPacket,1)
    };

    /**
     * @typedef NaluPacketCollection
     * @brief A collection (vector) of NaluPacket objects.
     */
    typedef std::vector<NaluPacket> NaluPacketCollection;

} // namespace dataProducts

#endif // NALUPACKET_HH
