/*!
    @file
    @brief BlockyCoderFile
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#include "blockycoderfile.h"
#include <unistd.h>

using namespace blocky;

BlockyCoderFile::BlockyCoderFile() :
    BlockyCoder()
{

}

BlockyCoderFile::BlockyCoderFile(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath) :
    BlockyCoder(_blockSize, _blocksPerGeneration, _dataLength),
    filePath(_filePath),
    file(NULL)
{
    buffer = new uint8_t[bufferSize];
    createBlocks(false);

}

BlockyCoderFile::BlockyCoderFile(BlockyCoderFile&& other)
    : BlockyCoderFile()
{

    swap(*this, other);

}

BlockyCoderFile::~BlockyCoderFile()
{

    if (buffer) {
        delete [] buffer;
    }

    if (file) {
        fclose(file);
        file = NULL;
    }
}

BlockyCoderFile& BlockyCoderFile::operator =(BlockyCoderFile& other)
{

    swap(*this, other);
    return *this;

}

BlockyCoderFile& BlockyCoderFile::operator =(BlockyCoderFile&& other)
{

    swap(*this, other);
    return *this;

}

void BlockyCoderFile::swap(BlockyCoderFile& first, BlockyCoderFile& second)
{

    BlockyCoder::swap(first, second);
    using std::swap;
    swap(first.filePath, second.filePath);
    swap(first.file, second.file);
    swap(first.fd, second.fd);

}

bool BlockyCoderFile::flushGeneration(size_t generation)
{

    if (!coders[generation].getDecoded()) {
        return false;
    }

    size_t offset = generation * blocksPerGeneration * blockSize;
    if (fseek(file, offset, SEEK_SET)) {
        throw system_error(errno, system_category());
    }

    if (fwrite(&buffer[generation * blocksPerGeneration * blockSize], blockSize, coders[generation].getNumBlocks(), file) != coders[generation].getNumBlocks()) {
        throw system_error(errno, system_category());
    }

    if (fflush(file)) {
        throw system_error(errno, system_category());
    }

    if (fsync(fd)) {
        throw system_error(errno, system_category());
    }

    return true;

}

BlockyCoderFile BlockyCoderFile::createEncoder(size_t _blockSize, size_t _blocksPerGeneration, string _filePath)
{

    FILE *file = fopen(_filePath.c_str(), "r+b");
    if (!file) {
        throw system_error(errno, system_category());
    }

    if (fseek(file, 0L, SEEK_END)) {
        throw system_error(errno, system_category());
    }
    size_t _dataLength = ftell(file);
    rewind(file);

    BlockyCoderFile encoder(_blockSize, _blocksPerGeneration, _dataLength, _filePath);

    if (_dataLength != fread(encoder.buffer, 1, _dataLength, file)) {
        throw system_error(errno, system_category());
    }

    encoder.file = file;
    encoder.fd = fileno(file);
    memset(encoder.buffer + _dataLength, 0, encoder.bufferSize - _dataLength);
    encoder.createEncoders();
    return encoder;

}

BlockyCoderFile BlockyCoderFile::createDecoder(size_t _blockSize, size_t _blocksPerGeneration, size_t _dataLength, string _filePath)
{

    FILE *file = fopen(_filePath.c_str(), "w+b");
    if (!file) {
        throw system_error(errno, system_category());
    }

    char c = 0;
    if (fseek(file, _dataLength - 1, SEEK_SET)) {
        throw system_error(errno, system_category());
    }

    if (1 != fwrite(&c, sizeof(char), 1, file)) {
        throw system_error(errno, system_category());
    }

    if (fflush(file)) {
        throw system_error(errno, system_category());
    }

    BlockyCoderFile decoder(_blockSize, _blocksPerGeneration, _dataLength, _filePath);
    decoder.file = file;
    decoder.fd = fileno(file);
    memset(decoder.buffer, 0, decoder.bufferSize);
    decoder.createDecoders();
    return decoder;

}
