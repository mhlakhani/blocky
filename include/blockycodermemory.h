/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _BLOCKYCODERMEMORY_H
#define _BLOCKYCODERMEMORY_H

#include "blockycoder.h"

namespace blocky {

class BlockyCoderMemory : public BlockyCoder {

public:

    BlockyCoderMemory();
    BlockyCoderMemory(BlockyCoderMemory&& other);
    ~BlockyCoderMemory();

    BlockyCoderMemory& operator=(BlockyCoderMemory& other);
    BlockyCoderMemory& operator=(BlockyCoderMemory&& other);

    static BlockyCoderMemory createEncoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, uint8_t *_buffer);
    static BlockyCoderMemory createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength);

protected:

    BlockyCoderMemory(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength);
    BlockyCoderMemory(const BlockyCoderMemory& other) = delete;

    void swap(BlockyCoderMemory& first, BlockyCoderMemory& second);

};

}

#endif
