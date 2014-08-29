/*!
    @file
    @brief Utility functions
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _UTILS_H
#define _UTILS_H

#include <cstdio>
#include <cstdint>
#include <string>

#include "blockycoder.h"
#include "blockycodermemory.h"
#include "blockycoderfile.h"
#include "blockycodermmap.h"

namespace blocky {

/*! @brief Utility functions */
class Utils {

public:

    /*! @brief Prints an n by n matrix
        @param[in] coeffs The matrix to print
        @param[in] n The size of the matrix
    */
    static void printMatrix(uint8_t **coeffs, size_t n);

    /*! @brief Prints an array of blocks
        @param[in] blocks The array of blocks
        @param[in] n The number of blocks
        @param[in] blockSize The block size
    */
    static void printBlocks(uint8_t **blocks, size_t n, size_t blockSize);

    /*! @brief Prints a block
        @param[in] block The block
        @param[in] blockSize The block size
    */
    static void printBlock(uint8_t *block, size_t blockSize);

    /*! @brief Creates a BlockyCoder of the appropriate type for encoding.
        @param[in] blockSize The block size
        @param[in] blocksPerGeneration The number of blocks per generation
        @param[in] dataLength The data length
        @param[in] fileName The file name
        @param[in] buffer The data buffer
        @returns A BlockyCoder of the appropriate type, usable for encoding.
    */
    template <typename T> static T createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer);

    /*! @brief Creates a BlockyCoder of the appropriate type for decoding.
        @param[in] blockSize The block size
        @param[in] blocksPerGeneration The number of blocks per generation
        @param[in] dataLength The data length
        @param[in] fileName The file name
        @param[in] buffer The data buffer
        @returns A BlockyCoder of the appropriate type, usable for decoding.
    */
    template <typename T> static T createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer);

};

}

#endif
