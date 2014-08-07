/*!
    @file
    @brief BlockyCoder
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#include "blockycoder.h"

using namespace blocky;

BlockyCoder::BlockyCoder() :
    blockSize(0),
    blocksPerGeneration(0),
    dataLength(0),
    bufferSize(0),
    numBlocks(0),
    numGenerations(0),
    decoded(false),
    partialLastBlock(false),
    partialLastGeneration(0),
    blocks(NULL),
    buffer(NULL),
    coders(NULL)
{

}

BlockyCoder::BlockyCoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength) :
    blockSize(_blockSize),
    blocksPerGeneration(_blocksPerGeneration),
    dataLength(_dataLength),
    bufferSize(0),
    numBlocks(dataLength / blockSize),
    numGenerations(numBlocks / blocksPerGeneration),
    decoded(false),
    partialLastBlock(false),
    partialLastGeneration(false),
    blocks(NULL),
    buffer(NULL),
    coders(NULL)
{

    if ((dataLength % blockSize) != 0) {
        partialLastBlock = true;
        numBlocks++;
    }

    if ((numBlocks % blocksPerGeneration) != 0) {
        partialLastGeneration = true;
        numGenerations++;
    }

    bufferSize = numBlocks * blockSize;

}

BlockyCoder::BlockyCoder(BlockyCoder&& other)
    : BlockyCoder()
{

    swap(*this, other);

}

BlockyCoder::~BlockyCoder()
{

    if (blocks) {

        // Have we allocated a separate last block?
        if (blocks[numBlocks-1] != &buffer[(numBlocks - 1) * blockSize]) {
            delete [] blocks[numBlocks - 1];
        }

        delete [] blocks;
    }

    if (coders) {
        delete [] coders;
    }

}

BlockyCoder& BlockyCoder::operator =(BlockyCoder& other)
{

    swap(*this, other);
    return *this;

}

BlockyCoder& BlockyCoder::operator =(BlockyCoder&& other)
{

    swap(*this, other);
    return *this;

}

void BlockyCoder::swap(BlockyCoder& first, BlockyCoder& second) 
{

    using std::swap;
    swap(first.blockSize, second.blockSize);
    swap(first.blocksPerGeneration, second.blocksPerGeneration);
    swap(first.dataLength, second.dataLength);
    swap(first.bufferSize, second.bufferSize);
    swap(first.numBlocks, second.numBlocks);
    swap(first.numGenerations, second.numGenerations);
    swap(first.decoded, second.decoded);
    swap(first.partialLastBlock, second.partialLastBlock);
    swap(first.partialLastGeneration, second.partialLastGeneration);
    swap(first.blocks, second.blocks);
    swap(first.buffer, second.buffer);
    swap(first.coders, second.coders);

}

bool BlockyCoder::store(BlockyPacket& packet) 
{

    if (packet.generation > getNumGenerations()) {
        return false;
    }

    if (packet.numBlocks != coders[packet.generation].getNumBlocks()) {
        return false;
    }

    if (coders[packet.generation].canDecode()) {
        return true;
    }

    size_t blockNo = (blocksPerGeneration * packet.generation) + coders[packet.generation].getRank();
    size_t blockSize = coders[packet.generation].getBlockSize();

    if (packet.blockSize != blockSize) {
        return false;
    }

    memcpy(blocks[blockNo], packet.data, blockSize);

    return coders[packet.generation].store(blocks[blockNo], packet.coeffs);

}

bool BlockyCoder::decodeGeneration(size_t generation) 
{

    if (generation > getNumGenerations()) {
        return false;
    }

    if (!canDecodeGeneration(generation)) {
        return false;
    }

    return coders[generation].decode();

}

bool BlockyCoder::decode() 
{

    bool retval = true;

    for (size_t i = 0; i < getNumGenerations(); i++) {

        if (!decodeGeneration(i)) {
            retval = false;
        }
    }

    return retval;

}

bool BlockyCoder::encode(BlockyPacket& packet, size_t generation) 
{

    if (generation > getNumGenerations()) {
        return false;
    }

    packet.generation = generation;
    packet.numBlocks = coders[generation].getNumBlocks();
    packet.blockSize = coders[generation].getBlockSize();

    if (packet.data == NULL) {
        packet.data = new uint8_t[packet.blockSize];
    }

    if (packet.coeffs == NULL) {
        packet.coeffs = new uint8_t[packet.numBlocks];
    }

    return coders[generation].encode(packet.data, packet.coeffs);

}

bool BlockyCoder::flushGeneration(size_t generation)
{
    (void) generation;
    return true;
}

bool BlockyCoder::flush()
{

    bool retval = true;

    for (size_t i = 0; i < getNumGenerations(); i++) {

        if (!flushGeneration(i)) {
            retval = false;
        }

    }

    return retval;
}

size_t BlockyCoder::getGenerationsCompleted() 
{

    size_t result = 0;
    for (size_t i = 0; i < getNumGenerations(); i++) {
        if (coders[i].canDecode()) {
            result++;
        }
    }
    return result;

}

void BlockyCoder::createBlocks(bool createLastBlockSeparately) {

    blocks = new uint8_t*[numBlocks];
    for (size_t i = 0; i < numBlocks; i++) {
        blocks[i] = &buffer[i * blockSize];
    }

    if (createLastBlockSeparately) {
        blocks[numBlocks-1] = new uint8_t[blockSize];
        memset(blocks[numBlocks-1], 0, blockSize);
    }

}

void BlockyCoder::createEncoders()
{

    coders = new Coder[numGenerations];
    for (size_t i = 0; i < numGenerations; i++) {
        if ((i == numGenerations-1) && partialLastGeneration) {
            coders[i] = Coder::createEncoder(blockSize, numBlocks - (i * blocksPerGeneration), &blocks[i * blocksPerGeneration]);
        } else {
            coders[i] = Coder::createEncoder(blockSize, blocksPerGeneration, &blocks[i * blocksPerGeneration]);
        }
    }

}

void BlockyCoder::createDecoders()
{

    coders = new Coder[numGenerations];
    for (size_t i = 0; i < numGenerations; i++) {
        if ((i == numGenerations-1) && partialLastGeneration) {
            coders[i] = Coder::createDecoder(blockSize, numBlocks - (i * blocksPerGeneration));
        } else {
            coders[i] = Coder::createDecoder(blockSize, blocksPerGeneration);
        }
    }

}
