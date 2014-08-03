/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _BLOCKYCODERFILE_H
#define _BLOCKYCODERFILE_H

#include <cstdio>
#include <exception>
#include <system_error>
#include <string>
#include "blockycoder.h"

using namespace std;

namespace blocky {

class BlockyCoderFile : public BlockyCoder {

public:

    BlockyCoderFile();
    BlockyCoderFile(BlockyCoderFile&& other);
    ~BlockyCoderFile();

    BlockyCoderFile& operator=(BlockyCoderFile& other);
    BlockyCoderFile& operator=(BlockyCoderFile&& other);

    static BlockyCoderFile createEncoder(size_t _blockSize, size_t _blocksPerGeneration, string _filePath);
    static BlockyCoderFile createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);

    inline string getFilePath() { return filePath; }
    bool flushGeneration(size_t generation);

protected:

    BlockyCoderFile(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);
    BlockyCoderFile(const BlockyCoderFile& other) = delete;

    void swap(BlockyCoderFile& first, BlockyCoderFile& second);

    string filePath;
    FILE *file;
    int fd;

};

}

#endif
