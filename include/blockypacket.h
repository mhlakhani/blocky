/*!
    @file
    @brief BlockyPacket
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _BLOCKYPACKET_H
#define _BLOCKYPACKET_H

namespace blocky {

/*! @brief Network Coded Packet */
struct BlockyPacket {

    /*! @brief The generation this packet is from */
    size_t generation;

    /*! @brief The number of blocks in the generation

        Should equal sizeof(#coeffs)
    */
    size_t numBlocks;

    /*! @brief The block size of the packet

        Should equal sizeof(#data)
    */
    size_t blockSize;

    /*! @brief The data inside the block */
    uint8_t *data;

    /*! @brief The coefficient vector */
    uint8_t *coeffs;

    /*! @brief Constructor */
    BlockyPacket() :
        generation(0),
        numBlocks(0),
        blockSize(0),
        data(NULL),
        coeffs(NULL)
    {

    }
};

}

#endif
