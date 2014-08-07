/*!
    @file
    @brief BlockyCoderFile
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _BLOCKYCODERFILE_H
#define _BLOCKYCODERFILE_H

#include <cstdio>
#include <exception>
#include <system_error>
#include <string>
#include "blockycoder.h"

using namespace std;

namespace blocky {

/*! @brief Network Coding Operations over a File

    Encodes and Decodes Files
*/
class BlockyCoderFile : public BlockyCoder {

public:

    /*! @brief Default constructor */
    BlockyCoderFile();

    /*! @brief Move constructor */
    BlockyCoderFile(BlockyCoderFile&& other);

    /*! @brief Destructor */
    ~BlockyCoderFile();

    /*! @brief Assignment operator */
    BlockyCoderFile& operator=(BlockyCoderFile& other);

    /*! @brief Move operator */
    BlockyCoderFile& operator=(BlockyCoderFile&& other);

    /*! @brief Creates an encoder
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _filePath The path of the file to encode
    */
    static BlockyCoderFile createEncoder(size_t _blockSize, size_t _blocksPerGeneration, string _filePath);

    /*! @brief Creates a decoder
        @param[in] _blockSize The block size
        @param[in] _blocksPerGeneration The number of blocks per generation
        @param[in] _dataLength The length of the incoming data file
        @param[in] _filePath The path of the file to store decoded data in
    */
    static BlockyCoderFile createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);

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
    BlockyCoderFile(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath);

    /*! @brief Copy constructor */
    BlockyCoderFile(const BlockyCoderFile& other) = delete;

    /*! @brief Swaps two BlockyCoderFile objects
        @param[in,out] first The first BlockyCoderFile
        @param[in,out] second The second BlockyCoderFile
    */
    void swap(BlockyCoderFile& first, BlockyCoderFile& second);

    /*! @brief The file path */
    string filePath;

    /*! @brief The file struct */
    FILE *file;

    /*! @brief The file descriptor */
    int fd;

};

}

#endif
