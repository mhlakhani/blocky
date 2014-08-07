/*!
    @file
    @brief Coder
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#include "coder.h"
#include <algorithm>

using namespace blocky;

Coder::Coder() :
    decoded(false),
    blockSize(0),
    numBlocks(0),
    rank(0),
    coeffs(NULL),
    blocks(NULL)
{

}

Coder::Coder(size_t _blockSize, size_t _numBlocks) :
    decoded(false),
    blockSize(_blockSize),
    numBlocks(_numBlocks),
    rank(0),
    coeffs(NULL),
    blocks(NULL)
{

    coeffs = new uint8_t*[numBlocks];
    for (size_t i = 0; i < numBlocks; i++) {
        coeffs[i] = new uint8_t[numBlocks];
        memset(coeffs[i], 0, numBlocks);
    }

    blocks = new uint8_t*[numBlocks];

}

Coder::Coder(size_t _blockSize, size_t _numBlocks, uint8_t **_blocks) :
    decoded(true),
    blockSize(_blockSize),
    numBlocks(_numBlocks),
    rank(_numBlocks),
    coeffs(NULL),
    blocks(NULL)
{

    coeffs = new uint8_t*[numBlocks];
    blocks = new uint8_t*[numBlocks];
    for (size_t i = 0; i < numBlocks; i++) {
        coeffs[i] = new uint8_t[numBlocks];
        memset(coeffs[i], 0, numBlocks);
        blocks[i] = _blocks[i];
        coeffs[i][i] = 1;
    }

}


Coder::Coder(const Coder& other) :
    decoded(other.decoded),
    blockSize(other.blockSize),
    numBlocks(other.numBlocks),
    rank(other.rank),
    coeffs(NULL),
    blocks(NULL)
{

    coeffs = new uint8_t*[numBlocks];
    blocks = new uint8_t*[numBlocks];
    for (size_t i = 0; i < numBlocks; i++) {
        coeffs[i] = new uint8_t[numBlocks];
        memcpy(coeffs[i], other.coeffs[i], numBlocks);
        blocks[i] = other.blocks[i];
    }

}

Coder::Coder(Coder&& other)
    : Coder()
{

    swap(*this, other);

}

Coder::~Coder() 
{

    if (coeffs) {
        for (size_t i = 0; i < numBlocks; i++) {
            delete [] coeffs[i];
        }

        delete [] coeffs;
    }

    if (blocks) {
        delete [] blocks;
    }
}

Coder& Coder::operator =(Coder& other)
{

    swap(*this, other);
    return *this;

}

Coder& Coder::operator =(Coder&& other)
{

    swap(*this, other);
    return *this;

}

void Coder::swap(Coder& first, Coder& second) 
{

    using std::swap;
    swap(first.decoded, second.decoded);
    swap(first.blockSize, second.blockSize);
    swap(first.numBlocks, second.numBlocks);
    swap(first.rank, second.rank);
    swap(first.coeffs, second.coeffs);
    swap(first.blocks, second.blocks);

}

Coder Coder::createEncoder(size_t _blockSize, size_t _numBlocks, uint8_t **_blocks) 
{

    return Coder(_blockSize, _numBlocks, _blocks);

}

Coder Coder::createDecoder(size_t _blockSize, size_t _numBlocks) 
{

    return Coder(_blockSize, _numBlocks);

}

bool Coder::store(uint8_t *block, uint8_t *_coeffs) 
{

    if (canDecode()) {
        return true;
    }

    bool increasesRank = gaussianElimination(_coeffs);
    if (!increasesRank) {
        return false;
    }

    // Gaussian elimination would have upped rank
    blocks[rank-1] = block;
    rowOperations();

    return true;
}

bool Coder::decode() 
{

    if (decoded) {
        return true;
    }

    if (!canDecode()) {
        return false;
    }

    backSubstitution();

    for (size_t i = 0; i < numBlocks; i++) {
        for (size_t j = 0; j < numBlocks; j++) {
            if (i == j) {
                coeffs[i][j] = 1;
            } else {
                coeffs[i][j] = 0;
            }
        }
    }

    decoded = true;
    return true;

}

bool Coder::encode(uint8_t *block, uint8_t *_coeffs) 
{

    if (rank == 0) {
        return false;
    }

    uint8_t *mcoeffs = new uint8_t[numBlocks];

    memset(mcoeffs, 0, numBlocks);
    memset(_coeffs, 0, numBlocks);
    memset(block, 0, blockSize);

    for (size_t i = 0; i < rank; i++) {
        while ((mcoeffs[i] = (uint8_t) (rand() % 256)) == 0);
    }

    for (size_t i = 0; i < rank; i++) {

        gf.addMultiple(mcoeffs[i], block, blocks[i], blockSize);
        gf.addMultiple(mcoeffs[i], _coeffs, coeffs[i], numBlocks);

    }

    delete [] mcoeffs;

    return true;
}

bool Coder::gaussianElimination(uint8_t *_coeffs) 
{

    if (canDecode()) {
        return false;
    }

    // Copy the coeffs buffer to work on
    uint8_t **cfs = new uint8_t*[numBlocks];
    for (size_t i = 0; i < numBlocks; i++) {
        cfs[i] = new uint8_t[numBlocks];
        memcpy(cfs[i], coeffs[i], numBlocks);
    }
    memcpy(cfs[rank], _coeffs, numBlocks);


    // Gaussian elimination
    for (size_t k = 0; k < numBlocks - 1; k++) {

        uint8_t ckk = cfs[k][k];
        for (size_t i = k+1; i < numBlocks; i++) {
            uint8_t cik = gf.div(gf.sub(0, cfs[i][k]), ckk);
            cfs[i][k] = cik;

            for (size_t j = k+1; j < numBlocks; j++) {
                cfs[i][j] = gf.add(cfs[i][j], gf.mul(cik, cfs[k][j]));
            }
        }

    }

    // Compute new rank
    size_t _rank = 0;
    for (size_t i = 0; i < numBlocks; i++) {
        // TODO: Be less strict and reject fewer packets?
        //for (size_t j = i; j < numBlocks; j++) {
            if (cfs[i][i] != 0) {
                _rank++;
                //break;
            //}
        }
    }

    // Clean up and copy new coeffs if needed
    for (size_t i = 0; i < numBlocks; i++) {
        if (_rank > rank) {
            memcpy(coeffs[i], cfs[i], numBlocks);
        }
        delete [] cfs[i];
    }
    delete [] cfs;

    if (_rank > rank) {
        rank = _rank;
        return true;
    } else {
        return false;
    }

}

void Coder::rowOperations() 
{

    if (rank < 2) {
        return;
    }

    for (size_t k = 0; k < rank-1; k++) {

        for (size_t i = k+1; i < rank; i++) {
            gf.addMultiple(coeffs[i][k], blocks[i], blocks[k], blockSize);
        }
    }

    for (size_t i = 0; i < rank; i++) {
        for (size_t j = 0; j < i; j++) {
            coeffs[i][j] = 0;
        }
    }

}

void Coder::backSubstitution() 
{

    if (numBlocks == 0) {
        return;
    }

    gf.div(coeffs[numBlocks-1][numBlocks-1], blocks[numBlocks-1], blockSize);

    if (numBlocks == 1) {
        return;
    }

    for (long i = numBlocks-2; i >= 0; i--) {
        for (size_t j = i+1; j < numBlocks; j++) {
            gf.subMultiple(coeffs[i][j], blocks[i], blocks[j], blockSize);
        }
        gf.div(coeffs[i][i], blocks[i], blockSize);
    }

}
