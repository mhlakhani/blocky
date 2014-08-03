/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _BLOCKYCODER_H
#define _BLOCKYCODER_H

#include <algorithm>
#include "blockypacket.h"
#include "coder.h"

namespace blocky {

class BlockyCoder {

public:

    bool store(BlockyPacket& packet);
    bool decodeGeneration(size_t generation);
    bool decode();
    bool encode(BlockyPacket& packet, size_t generation);
    virtual bool flushGeneration(size_t generation);
    bool flush();

    inline size_t getBlockSize() { return blockSize; }
    inline size_t getBlocksPerGeneration() { return blocksPerGeneration; }
    inline size_t getDataLength() { return dataLength; }
    inline size_t getBufferSize() { return bufferSize; }
    inline size_t getNumBlocks() { return numBlocks; }
    inline size_t getNumBlocksInGeneration(size_t i) { return coders[i].getNumBlocks(); }
    inline size_t getNumGenerations() { return numGenerations; }
    inline bool getPartialLastBlock() { return partialLastBlock; }
    inline bool getPartialLastGeneration() { return partialLastGeneration; }
    inline uint8_t *getBuffer() { return buffer; }

    size_t getGenerationsCompleted();
    inline bool canDecode() { return (getNumGenerations() == getGenerationsCompleted()); }
    inline size_t getRank(size_t generation) { return coders[generation].getRank(); }
    inline bool canDecodeGeneration(size_t generation) { return coders[generation].canDecode(); }
    inline bool getDecoded() { return decoded; }
    inline bool getGenerationDecoded(size_t generation) { return coders[generation].getDecoded(); }

protected:

    BlockyCoder();
    BlockyCoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength);
    BlockyCoder(const BlockyCoder& other) = delete;
    BlockyCoder(BlockyCoder&& other);
    ~BlockyCoder();

    BlockyCoder& operator=(BlockyCoder& other);
    BlockyCoder& operator=(BlockyCoder&& other);

    void swap(BlockyCoder& first, BlockyCoder& second);

    void createBlocks(bool createLastBlockSeparately);
    void createEncoders();
    void createDecoders();

    size_t blockSize;
    size_t blocksPerGeneration;
    size_t dataLength;
    size_t bufferSize;
    size_t numBlocks;
    size_t numGenerations;
    bool decoded;
    bool partialLastBlock;
    bool partialLastGeneration;
    uint8_t **blocks;
    uint8_t *buffer;
    Coder *coders;
};

}

#endif
