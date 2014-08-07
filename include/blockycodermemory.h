/*!
    @file
    @brief BlockyCoderMemory
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _BLOCKYCODERMEMORY_H
#define _BLOCKYCODERMEMORY_H

#include "blockycoder.h"

namespace blocky {

/*! @brief Network Coding Operations over a Memory Buffer

    Encodes and Decodes Memory Buffers
*/
class BlockyCoderMemory : public BlockyCoder {

public:

    /*! @brief Default constructor */
    BlockyCoderMemory();

    /*! @brief Move constructor */
    BlockyCoderMemory(BlockyCoderMemory&& other);

    /*! @brief Destructor */
    ~BlockyCoderMemory();

    /*! @brief Assignment operator */
    BlockyCoderMemory& operator=(BlockyCoderMemory& other);

    /*! @brief Move operator */
    BlockyCoderMemory& operator=(BlockyCoderMemory&& other);

    /*! @brief Creates an encoder
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _dataLength The length of the buffer to encode
        @param[in,out] _buffer The buffer to encode
    */
    static BlockyCoderMemory createEncoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, uint8_t *_buffer);

    /*! @brief Creates a decoder
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _dataLength The length of the data to decode
    */
    static BlockyCoderMemory createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength);

protected:

    /*! @brief Base constructor
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _dataLength The data length
    */
    BlockyCoderMemory(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength);

    /*! @brief Copy constructor */
    BlockyCoderMemory(const BlockyCoderMemory& other) = delete;

    /*! @brief Swaps two BlockyCoderMemory objects
        @param[in,out] first The first BlockyCoderMemory
        @param[in,out] second The second BlockyCoderMemory
    */
    void swap(BlockyCoderMemory& first, BlockyCoderMemory& second);

};

}

#endif
