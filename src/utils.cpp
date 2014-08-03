/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
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

