/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _BLOCKYCODERMMAP_H
#define _BLOCKYCODERMMAP_H

#include <cstdio>
#include <exception>
#include <system_error>
#include <string>
#include "blockycoder.h"

using namespace std;

namespace blocky {

class BlockyCoderMmap : public BlockyCoder {

public:

    BlockyCoderMmap();
    BlockyCoderMmap(BlockyCoderMmap&& other);
    ~BlockyCoderMmap();

    BlockyCoderMmap& operator=(BlockyCoderMmap& other);
    BlockyCoderMmap& operator=(BlockyCoderMmap&& other);

    static BlockyCoderMmap createEncoder(size_t _blockSize, size_t _blocksPerGeneration, string _filePath);
    static BlockyCoderMmap createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);

    inline string getFilePath() { return filePath; }
    bool flushGeneration(size_t generation);

protected:

    BlockyCoderMmap(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);
    BlockyCoderMmap(const BlockyCoderMmap& other) = delete;

    void swap(BlockyCoderMmap& first, BlockyCoderMmap& second);

    string filePath;
    FILE *file;
    int fd;

    const static off_t pageSize;

};

}

#endif
