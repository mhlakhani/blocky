/*!
    @file
    @brief BlockyCoderMmap
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _BLOCKYCODERMMAP_H
#define _BLOCKYCODERMMAP_H

#include <cstdio>
#include <exception>
#include <system_error>
#include <string>
#include "blockycoder.h"

using namespace std;

namespace blocky {

/*! @brief Network Coding Operations over a memory mapped file

    Encodes and Decodes Files by mapping them into memory
*/
class BlockyCoderMmap : public BlockyCoder {

public:

    /*! @brief Default constructor */
    BlockyCoderMmap();

    /*! @brief Move constructor */
    BlockyCoderMmap(BlockyCoderMmap&& other);

    /*! @brief Destructor */
    ~BlockyCoderMmap();

    /*! @brief Assignment operator */
    BlockyCoderMmap& operator=(BlockyCoderMmap& other);

    /*! @brief Move operator */
    BlockyCoderMmap& operator=(BlockyCoderMmap&& other);

    /*! @brief Creates an encoder
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _filePath The path of the file to encode
    */
    static BlockyCoderMmap createEncoder(size_t _blockSize, size_t _blocksPerGeneration, string _filePath);

    /*! @brief Creates a decoder
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _dataLength The length of the incoming data file
        @param[in] _filePath The path of the file to store decoded data in
    */
    static BlockyCoderMmap createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);

    /*! @brief Get the file path
        @returns The file path
    */
    inline string getFilePath() { return filePath; }

    /*! @brief Flushes the decoded data to the output
        @param[in] generation The generation to flush
        @returns true on success, false on error
    */
    bool flushGeneration(size_t generation);

protected:

    /*! @brief Base constructor
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _dataLength The data length
        @param[in] _filePath The file path
    */
    BlockyCoderMmap(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);

    /*! @brief Copy constructor */
    BlockyCoderMmap(const BlockyCoderMmap& other) = delete;

    /*! @brief Swaps two BlockyCoderFile objects
        @param[in,out] first The first BlockyCoderFile
        @param[in,out] second The second BlockyCoderFile
    */
    void swap(BlockyCoderMmap& first, BlockyCoderMmap& second);

    /*! @brief The file path */
    string filePath;

    /*! @brief The file struct */
    FILE *file;

    /*! @brief The file descriptor */
    int fd;

    /*! @brief The page size */
    const static off_t pageSize;

};

}

#endif
