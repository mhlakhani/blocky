/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _CODER_H
#define _CODER_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "gf28.h"

namespace blocky {

class Coder {

public:

    Coder();
    Coder(const Coder& other);
    Coder(Coder&& other);
    ~Coder();

    Coder& operator=(Coder& other);
    Coder& operator=(Coder&& other);

    static Coder createEncoder(size_t _blockSize, size_t _numBlocks, uint8_t **_blocks);
    static Coder createDecoder(size_t _blockSize, size_t _numBlocks);

    bool store(uint8_t *block, uint8_t *_coeffs);
    bool decode();
    bool encode(uint8_t *block, uint8_t *_coeffs);

    inline bool getDecoded() { return decoded; }
    inline size_t getBlockSize() { return blockSize; }
    inline size_t getNumBlocks() { return numBlocks; }
    inline size_t getRank() { return rank; }
    inline bool canDecode() { return (rank == numBlocks); }
    inline uint8_t* operator[] (const int i) { return blocks[i]; }
    inline uint8_t** getBlocks() { return blocks; }

private:

    Coder(size_t _blockSize, size_t _numBlocks, uint8_t **_blocks); // Encoder
    Coder(size_t _blockSize, size_t _numBlocks); // Decoder

    static void swap(Coder& first, Coder& second);

    bool gaussianElimination(uint8_t *_coeffs);
    void rowOperations();
    void backSubstitution();

    bool decoded;
    size_t blockSize;
    size_t numBlocks;
    size_t rank;
    uint8_t **coeffs;
    uint8_t **blocks;
    GF28 gf;
};

}

#endif
