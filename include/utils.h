/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _UTILS_H
#define _UTILS_H

#include <cstdio>
#include <cstdint>

namespace blocky {

class Utils {

public:

    static void printMatrix(uint8_t **coeffs, size_t n);
    static void printBlocks(uint8_t **blocks, size_t n, size_t blockSize);
    static void printBlock(uint8_t *block, size_t blockSize);

};

}

#endif
