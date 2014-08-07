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

};

}

#endif
