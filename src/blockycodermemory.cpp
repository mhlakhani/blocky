/*!
    @file
    @brief BlockyCoderMemory
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#include "blockycodermemory.h"

using namespace blocky;

BlockyCoderMemory::BlockyCoderMemory() :
    BlockyCoder()
{

}

BlockyCoderMemory::BlockyCoderMemory(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength) :
    BlockyCoder(_blockSize, _blocksPerGeneration, _dataLength)
{

    buffer = new uint8_t[bufferSize];
    createBlocks(false);

}

BlockyCoderMemory::BlockyCoderMemory(BlockyCoderMemory&& other)
    : BlockyCoderMemory()
{

    swap(*this, other);

}

BlockyCoderMemory::~BlockyCoderMemory()
{

    if (buffer) {
        delete [] buffer;
    }
}

BlockyCoderMemory& BlockyCoderMemory::operator =(BlockyCoderMemory& other)
{

    swap(*this, other);
    return *this;

}

BlockyCoderMemory& BlockyCoderMemory::operator =(BlockyCoderMemory&& other)
{

    swap(*this, other);
    return *this;

}

void BlockyCoderMemory::swap(BlockyCoderMemory& first, BlockyCoderMemory& second)
{

    BlockyCoder::swap(first, second);

}

BlockyCoderMemory BlockyCoderMemory::createEncoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, uint8_t *_buffer)
{

    BlockyCoderMemory encoder(_blockSize, _blocksPerGeneration, _dataLength);
    memcpy(encoder.buffer, _buffer, _dataLength);
    memset(encoder.buffer + _dataLength, 0, encoder.bufferSize - _dataLength);
    encoder.createEncoders();
    return encoder;

}

BlockyCoderMemory BlockyCoderMemory::createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength)
{

    BlockyCoderMemory decoder(_blockSize, _blocksPerGeneration, _dataLength);
    memset(decoder.buffer, 0, decoder.bufferSize);
    decoder.createDecoders();
    return decoder;

}
