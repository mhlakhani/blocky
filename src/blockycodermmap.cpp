/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#include "blockycodermmap.h"
#include <sys/mman.h>
#include <unistd.h>

using namespace blocky;

const off_t BlockyCoderMmap::pageSize = sysconf(_SC_PAGE_SIZE);

BlockyCoderMmap::BlockyCoderMmap() :
    BlockyCoder()
{

}

BlockyCoderMmap::BlockyCoderMmap(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath) :
    BlockyCoder(_blockSize, _blocksPerGeneration, _dataLength),
    filePath(_filePath),
    file(NULL),
    fd(0)
{

    bufferSize = dataLength;

}

BlockyCoderMmap::BlockyCoderMmap(BlockyCoderMmap&& other)
    : BlockyCoderMmap()
{

    swap(*this, other);

}

BlockyCoderMmap::~BlockyCoderMmap()
{

    if (file) {
        fclose(file);
        file = NULL;
        munmap(buffer, dataLength);
    }
}

BlockyCoderMmap& BlockyCoderMmap::operator =(BlockyCoderMmap& other)
{

    swap(*this, other);
    return *this;

}

BlockyCoderMmap& BlockyCoderMmap::operator =(BlockyCoderMmap&& other)
{

    swap(*this, other);
    return *this;

}

void BlockyCoderMmap::swap(BlockyCoderMmap& first, BlockyCoderMmap& second)
{

    BlockyCoder::swap(first, second);
    using std::swap;
    swap(first.filePath, second.filePath);
    swap(first.file, second.file);
    swap(first.fd, second.fd);

}


BlockyCoderMmap BlockyCoderMmap::createEncoder(size_t _blockSize, size_t _blocksPerGeneration, string _filePath)
{

    FILE *file = fopen(_filePath.c_str(), "r+b");
    if (!file) {
        throw system_error(errno, system_category());
    }

    if (fseek(file, 0L, SEEK_END)) {
        throw system_error(errno, system_category());
    }
    size_t _dataLength = ftell(file);
    rewind(file);

    int fd = fileno(file);
    uint8_t *buffer = (uint8_t *) mmap(NULL, _dataLength, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buffer == MAP_FAILED) {
        throw system_error(errno, system_category());
    }
    madvise(buffer, _dataLength, MADV_WILLNEED);
    madvise(buffer, _dataLength, MADV_SEQUENTIAL);

    BlockyCoderMmap encoder(_blockSize, _blocksPerGeneration, _dataLength, _filePath);
    encoder.file = file;
    encoder.fd = fd;
    encoder.buffer = buffer;

    encoder.createBlocks(encoder.partialLastBlock);
    if (encoder.partialLastBlock) {
        memset(encoder.blocks[encoder.numBlocks-1], 0, encoder.blockSize);
        memcpy(encoder.blocks[encoder.numBlocks-1], &buffer[(encoder.numBlocks-1) * encoder.blockSize], encoder.dataLength % encoder.blockSize);
    }
    encoder.createEncoders();
    return encoder;

}

BlockyCoderMmap BlockyCoderMmap::createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath)
{

    FILE *file = fopen(_filePath.c_str(), "w+b");
    if (!file) {
        throw system_error(errno, system_category());
    }

    char c = 0;
    if (fseek(file, _dataLength - 1, SEEK_SET)) {
        throw system_error(errno, system_category());
    }

    if (1 != fwrite(&c, sizeof(char), 1, file)) {
        throw system_error(errno, system_category());
    }

    if (fflush(file)) {
        throw system_error(errno, system_category());
    }
    rewind(file);

    int fd = fileno(file);
    uint8_t *buffer = (uint8_t *) mmap(NULL, _dataLength, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buffer == MAP_FAILED) {
        throw system_error(errno, system_category());
    }
    madvise(buffer, _dataLength, MADV_WILLNEED);
    madvise(buffer, _dataLength, MADV_SEQUENTIAL);

    BlockyCoderMmap decoder(_blockSize, _blocksPerGeneration, _dataLength, _filePath);
    decoder.file = file;
    decoder.fd = fd;
    decoder.buffer = buffer;

    decoder.createBlocks(decoder.partialLastBlock);
    decoder.createDecoders();
    return decoder;

}

bool BlockyCoderMmap::flushGeneration(size_t generation)
{

    if (!coders[generation].getDecoded()) {
        return false;
    }

    size_t offset = generation * blocksPerGeneration * blockSize;
    size_t delta = offset % pageSize;
    size_t length = blockSize * coders[generation].getNumBlocks();

    if (delta != 0) {
        offset -= delta;
        length += delta;
    }

    if (partialLastBlock && generation == (numGenerations - 1)) {
        memcpy(&buffer[offset + delta + length - blockSize], blocks[numBlocks-1], dataLength % blockSize);
        length -= (blockSize - (dataLength % blockSize));
    }
    
    if (msync(&buffer[offset], length, MS_ASYNC)) {
        throw system_error(errno, system_category());
    }

    return true;

}
