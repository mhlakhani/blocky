/*!
    @file
    @brief Coder
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _CODER_H
#define _CODER_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "gf28.h"

namespace blocky {

/*! @brief Low Level Network Coding Operations

    Operates directly on arrays

    @warning Not recommended for normal use. Use BlockyCoder instead
*/
class Coder {

public:

    /*! @brief Default constructor */
    Coder();

    /*! @brief Copy constructor */
    Coder(const Coder& other);

    /*! @brief Move constructor */
    Coder(Coder&& other);

    /*! @brief Destructor */
    ~Coder();

    /*! @brief Assignment operator */
    Coder& operator=(Coder& other);

    /*! @brief Move operator */
    Coder& operator=(Coder&& other);

    /*! @brief Creates an encoder
        @param[in] _blockSize The block size
        @param[in] _numBlocks The number of blocks
        @param[in] _blocks The blocks of data
        @warning The coder will use the blocks as is and not free them when destroyed. It is the responsibility of the caller to free the memory appropriately.
    */
    static Coder createEncoder(size_t _blockSize, size_t _numBlocks, uint8_t **_blocks);

    /*! @brief Creates a decoder
        @param[in] _blockSize The block size
        @param[in] _numBlocks The number of blocks
    */
    static Coder createDecoder(size_t _blockSize, size_t _numBlocks);

    /*! @brief Stores a block
        @param[in] block The block
        @param[in] _coeffs The coefficients
        @returns Whether the block was helpful
        @warning The coder will take ownership of the block as is and not free it when destroyed. It is the responsibility of the caller to free memory appropriately.
    */
    bool store(uint8_t *block, uint8_t *_coeffs);

    /*! @brief Decodes the data
        @returns Whether decoding succeeded
    */
    bool decode();

    /*! @brief Encodes a block
        @param[out] block The block (will be filled in)
        @param[out] _coeffs The coefficient vector (will be filled in)
        @returns Whether encoding succeeded
    */
    bool encode(uint8_t *block, uint8_t *_coeffs);

    /*! @brief Get the decoding status
        @returns Whether decoding has been completed
    */
    inline bool getDecoded() { return decoded; }

    /*! @brief Get the block size
        @returns The block size
    */
    inline size_t getBlockSize() { return blockSize; }

    /*! @brief Get the number of blocks
        @returns The number of blocks
    */
    inline size_t getNumBlocks() { return numBlocks; }

    /*! @brief Get the rank (number of linearly independent blocks)
        @returns The rank
    */
    inline size_t getRank() { return rank; }

    /*! @brief Get whether decoding can happen
        @returns Whether it is possible to decode or not
    */
    inline bool canDecode() { return (rank == numBlocks); }

    /*! @brief Get the i-th block
        @param[in] i The block to return
        @returns The i-th block
    */
    inline uint8_t* operator[] (const int i) { return blocks[i]; }

    /*! @brief Get the array of blocks
        @returns Array of blocks
    */
    inline uint8_t** getBlocks() { return blocks; }

private:

    /*! @brief Constructor for encoders
        @param[in] _blockSize The block size
        @param[in] _numBlocks The number of blocks
        @param[in] _blocks The blocks of data
    */
    Coder(size_t _blockSize, size_t _numBlocks, uint8_t **_blocks);

    /*! @brief Constructor for decoders
        @param[in] _blockSize The block size
        @param[in] _numBlocks The number of blocks
    */
    Coder(size_t _blockSize, size_t _numBlocks);

    /*! @brief Swaps two Coder objects
        @param[in,out] first The first Coder
        @param[in,out] second The second Coder
    */
    static void swap(Coder& first, Coder& second);

    /*! @brief Gaussian elimination to determine whether given coefficient vector is helpful
        @param[in] _coeffs The coefficients vector
        @returns Whether the coefficient vector raises the rank

        Adds the given coefficient vector to the last row of the coefficient matrix, then
        performs gaussian elimination.

        If the rank increases, the coefficient matrix is updated; otherwise no change is made.
    */
    bool gaussianElimination(uint8_t *_coeffs);

    /*! @brief Perform row operations on the blocks corresponding to the operations on the coefficient matrix. */
    void rowOperations();

    /*! @brief Perform back substitution to decode the blocks */
    void backSubstitution();

    /*! @brief Whether the data has been decoded */
    bool decoded;

    /*! @brief The block size */
    size_t blockSize;

    /*! @brief The number of blocks in the generation */
    size_t numBlocks;

    /*! @brief The rank (number of linearly independent blocks) */
    size_t rank;

    /*! @brief The coefficient matrix */
    uint8_t **coeffs;

    /*! @brief The array of blocks */
    uint8_t **blocks;

    /*! @brief The Galois Field object for finite field operations

        @see GF28
    */
    GF28 gf;
};

}

#endif
