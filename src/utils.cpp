/*!
    @file
    @brief Utility functions
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#include "utils.h"

using namespace blocky;

void Utils::printMatrix(uint8_t **coeffs, size_t n)
{

    for (size_t i = 0; i < n; i++) {
        printf("| ");
        for (size_t j = 0; j < n; j++) {
            printf("%3u ", coeffs[i][j]);
        }
        printf("|\n");
    }

}

void Utils::printBlocks(uint8_t **blocks, size_t n, size_t blockSize)
{

    for (size_t i = 0; i < n; i++) {
        printf("Block %lu (%p): ", i, blocks[i]);
        for (size_t j = 0; j < blockSize; j++) {
            printf("%3u ", blocks[i][j]);
        }
        printf("\n");
    }

}

void Utils::printBlock(uint8_t *block, size_t blockSize)
{

    Utils::printBlocks(&block, 1, blockSize);

}

template<> BlockyCoderMemory Utils::createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) fileName;
    return BlockyCoderMemory::createEncoder(blockSize, blocksPerGeneration, dataLength, buffer);
}

template<> BlockyCoderMemory Utils::createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) fileName;
    (void) buffer;
    return BlockyCoderMemory::createDecoder(blockSize, blocksPerGeneration, dataLength);
}

template<> BlockyCoderFile Utils::createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) dataLength;
    (void) buffer;
    return BlockyCoderFile::createEncoder(blockSize, blocksPerGeneration, fileName);
}

template<> BlockyCoderFile Utils::createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) buffer;
    return BlockyCoderFile::createDecoder(blockSize, blocksPerGeneration, dataLength, fileName);
}

template<> BlockyCoderMmap Utils::createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) dataLength;
    (void) buffer;
    return BlockyCoderMmap::createEncoder(blockSize, blocksPerGeneration, fileName);
}

template<> BlockyCoderMmap Utils::createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) buffer;
    return BlockyCoderMmap::createDecoder(blockSize, blocksPerGeneration, dataLength, fileName);
}

