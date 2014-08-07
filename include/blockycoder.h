/*!
    @file
    @brief BlockyCoder
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _BLOCKYCODER_H
#define _BLOCKYCODER_H

#include <algorithm>
#include "blockypacket.h"
#include "coder.h"

namespace blocky {

/*! @brief Network Coding Operations

    Given some data (file or in-memory buffer), encodes and decodes packets

    Wrapper over Coder

    @warning This is not supposed to be instantiated directly. Use one of the sub classes
*/
class BlockyCoder {

public:

    /*! @brief Stores a packet
        @param[in] packet The packet
        @returns true if the packet was helpful, false otherwise (or on error)
    */
    bool store(BlockyPacket& packet);

    /*! @brief Decodes the given generation
        @param[in] generation The generation to decode
        @returns true if decoding succeeded, false otherwise (or on error)
    */
    bool decodeGeneration(size_t generation);

    /*! @brief Decodes all generations
        @returns true if all the data was decoded, false otherwise (or on error)
    */
    bool decode();

    /*! @brief Encodes a packet from the given generation
        @param[in,out] packet The output packet (will be filled in)
        @param[in] generation The generation to encode a packet from
        @returns true on success, false on error

        @warning If the packet's data and/or coeffs fields are not null, they must be pointers to arrays of the correct length.
    */
    bool encode(BlockyPacket& packet, size_t generation);

    /*! @brief Flushes the decoded data to the output
        @param[in] generation The generation to flush
        @returns true on success, false on error
    */
    virtual bool flushGeneration(size_t generation);

    /*! @brief Flushes all generations to the output
        @returns true on success, false on error
    */
    bool flush();

    /*! @brief Get the block size
        @returns The block size
    */
    inline size_t getBlockSize() { return blockSize; }

    /*! @brief Get the number of blocks per generation
        @returns The number of blocks per generation
    */
    inline size_t getBlocksPerGeneration() { return blocksPerGeneration; }

    /*! @brief Get the data length
        @returns The data length
    */
    inline size_t getDataLength() { return dataLength; }

    /*! @brief Get the buffer size
        @returns The buffer size
    */
    inline size_t getBufferSize() { return bufferSize; }

    /*! @brief Get the number of blocks
        @returns The number of blocks
    */
    inline size_t getNumBlocks() { return numBlocks; }

    /*! @brief Get the number of blocks in the given generation
        @param[in] generation The generation
        @returns The number of blocks in the given generation
    */
    inline size_t getNumBlocksInGeneration(size_t generation) { return coders[generation].getNumBlocks(); }

    /*! @brief Get the number of generations
        @returns The number of generations
    */
    inline size_t getNumGenerations() { return numGenerations; }

    /*! @brief Get whether the last block is partial (due to padding)
        @returns Whether the last block is partial
    */
    inline bool getPartialLastBlock() { return partialLastBlock; }

    /*! @brief Get whether the last generation is partial (due to padding)
        @returns Whether the last generation is partial
    */
    inline bool getPartialLastGeneration() { return partialLastGeneration; }

    /*! @brief Get the underlying buffer
        @returns The underlying buffer
    */
    inline uint8_t *getBuffer() { return buffer; }

    /*! @brief Get the number of generations completed
        @returns The number of generations completed
    */
    size_t getGenerationsCompleted();

    /*! @brief Get whether it is possible to decode the data
        @returns Whether it is possible to decode the data
    */
    inline bool canDecode() { return (getNumGenerations() == getGenerationsCompleted()); }

    /*! @brief Get the rank of a given generation
        @param[in] generation The generation
        @returns The rank of the given generation
    */
    inline size_t getRank(size_t generation) { return coders[generation].getRank(); }

    /*! @brief Get whether it is possible to decode the given generation
        @param[in] generation The generation
        @returns Whether it is possible to decode the given generation
    */
    inline bool canDecodeGeneration(size_t generation) { return coders[generation].canDecode(); }

    /*! @brief Get whether the data has been decoded
        @returns Whether the data has been decoded
    */
    inline bool getDecoded() { return decoded; }

    /*! @brief Get whether the given generation has been decoded
        @param[in] generation The generation
        @returns Whether the given generation has been decoded
    */
    inline bool getGenerationDecoded(size_t generation) { return coders[generation].getDecoded(); }

protected:

    /*! @brief Default constructor */
    BlockyCoder();

    /*! @brief Base constructor
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _dataLength The data length
    */
    BlockyCoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength);

    /*! @brief Copy constructor */
    BlockyCoder(const BlockyCoder& other) = delete;

    /*! @brief Move constructor */
    BlockyCoder(BlockyCoder&& other);

    /*! @brief Destructor */
    ~BlockyCoder();

    /*! @brief Assignment operator */
    BlockyCoder& operator=(BlockyCoder& other);

    /*! @brief Move operator */
    BlockyCoder& operator=(BlockyCoder&& other);

    /*! @brief Swaps two BlockyCoder objects
        @param[in,out] first The first BlockyCoder
        @param[in,out] second The second BlockyCoder
    */
    void swap(BlockyCoder& first, BlockyCoder& second);

    /*! @brief Creates the array of blocks spanning the buffer
        @param[in] createLastBlockSeparately Whether the last block should use separate allocation
    */
    void createBlocks(bool createLastBlockSeparately);

    /*! @brief Creates a set of encoders over the blocks */
    void createEncoders();

    /*! @brief Creates a set of decoders over the blocks */
    void createDecoders();

    /*! @brief The block size */
    size_t blockSize;

    /*! @brief The number of blocks per generation */
    size_t blocksPerGeneration;

    /*! @brief The data length */
    size_t dataLength;

    /*! @brief The buffer size */
    size_t bufferSize;

    /*! @brief The number of blocks */
    size_t numBlocks;

    /*! @brief The number of generations */
    size_t numGenerations;

    /*! @brief Whether decoding has been completed */
    bool decoded;

    /*! @brief Whether the last block is partial */
    bool partialLastBlock;

    /*! @brief Whether the last generation is partial */
    bool partialLastGeneration;

    /*! @brief The array of blocks */
    uint8_t **blocks;

    /*! @brief The buffer */
    uint8_t *buffer;

    /*! @brief The array of coders over the buffer

        @see Coder
    */
    Coder *coders;
};

}

#endif
