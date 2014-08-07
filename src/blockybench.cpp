/*!
    @file
    @brief BlockyBench
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#include "gf28.h"
#include "utils.h"
#include "blockypacket.h"
#include "coder.h"
#include "blockycodermemory.h"
#include "blockycoderfile.h"
#include "blockycodermmap.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sys/time.h>

using namespace std;
using namespace blocky;

void freeBuffers(vector<uint8_t *> buffers) 
{

    for (size_t i = 0; i < buffers.size(); i++) {
        delete [] buffers[i];
    }

}

size_t timeDelta(struct timeval& start, struct timeval& end)
{

    return 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

}

template <typename T> T createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer);
template <typename T> T createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer);

template<> BlockyCoderMemory createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) fileName;
    return BlockyCoderMemory::createEncoder(blockSize, blocksPerGeneration, dataLength, buffer);
}

template<> BlockyCoderMemory createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) fileName;
    (void) buffer;
    return BlockyCoderMemory::createDecoder(blockSize, blocksPerGeneration, dataLength);
}

template<> BlockyCoderFile createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) dataLength;
    (void) buffer;
    return BlockyCoderFile::createEncoder(blockSize, blocksPerGeneration, fileName);
}

template<> BlockyCoderFile createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) buffer;
    return BlockyCoderFile::createDecoder(blockSize, blocksPerGeneration, dataLength, fileName);
}

template<> BlockyCoderMmap createBlockyEncoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) dataLength;
    (void) buffer;
    return BlockyCoderMmap::createEncoder(blockSize, blocksPerGeneration, fileName);
}

template<> BlockyCoderMmap createBlockyDecoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, string fileName, uint8_t *buffer)
{
    (void) buffer;
    return BlockyCoderMmap::createDecoder(blockSize, blocksPerGeneration, dataLength, fileName);
}

template <typename B> void benchCoder(const char *name, size_t blockSize, size_t blocksPerGeneration, size_t dataLength, size_t numIterations) 
{

    uint8_t *data = new uint8_t[dataLength];
    for (size_t i = 0; i < dataLength; i++) {
        uint8_t v = i;
        data[i] = v;
    }

    ofstream encfile("test.enc", ios::out|ios::binary);
    if (!encfile.is_open()) {
        printf("ERROR: Error opening temp file\n");
        return;
    }

    encfile.write((char *) data, dataLength);
    encfile.close();

    struct timeval start1, start2, start3, end1, end2, end3;
    vector<BlockyPacket> packets;
    vector<uint8_t *> buffers;
    vector<size_t> encodeTimePerPacket, encodeTimePerGeneration, decodeTimePerPacket, decodeTimePerGeneration, decodeTimePerFile, timePerFlush, totalTime;
    size_t averageEncodePerPacket, averageEncodePerGeneration, averageDecodePerPacket, averageDecodePerGeneration, averageDecodePerFile, averageFlush, averageTime;
    size_t minEncodePerPacket, minEncodePerGeneration, minDecodePerPacket, minDecodePerGeneration, minDecodePerFile, minFlush, minTime;
    size_t maxEncodePerPacket, maxEncodePerGeneration, maxDecodePerPacket, maxDecodePerGeneration, maxDecodePerFile, maxFlush, maxTime;
    size_t counter, numGenerations;

    for (size_t k = 0; k < numIterations; k++) {

        if (gettimeofday(&start3, NULL)) {
            goto err;
        }

        B encoder = createBlockyEncoder<B>(blockSize, blocksPerGeneration, dataLength, "test.enc", data);
        B decoder = createBlockyDecoder<B>(blockSize, blocksPerGeneration, dataLength, "test.dec", data);

        numGenerations = encoder.getNumGenerations();
        for (size_t i = 0; i < encoder.getNumGenerations(); i++) {

            if (gettimeofday(&start1, NULL)) {
                goto err;
            }

            for (size_t j = 0; j < (encoder.getNumBlocksInGeneration(i)+1); j++) {

                if (gettimeofday(&start2, NULL)) {
                    goto err;
                }

                BlockyPacket packet;
                if (!encoder.encode(packet, i)) {
                    printf("Error encoding packet (%lu, %lu)!\n", i, j);
                    goto err;
                }

                if (gettimeofday(&end2, NULL)) {
                    goto err;
                }

                buffers.push_back(packet.data);
                buffers.push_back(packet.coeffs);
                packets.push_back(packet);
                encodeTimePerPacket.push_back(timeDelta(start2, end2));

            }

            if (gettimeofday(&end1, NULL)) {
                goto err;
            }

            encodeTimePerGeneration.push_back(timeDelta(start1, end1));

        }

        counter = 0;
        decodeTimePerFile.push_back(0);
        for (size_t i = 0; i < encoder.getNumGenerations(); i++) {

            for (size_t j = 0; j < (encoder.getNumBlocksInGeneration(i)+1); j++) {

                if (gettimeofday(&start2, NULL)) {
                    goto err;
                }

                if (!decoder.store(packets[counter++])) {
                    // Ignore, this can happen
                }

                if (gettimeofday(&end2, NULL)) {
                    goto err;
                }

                decodeTimePerPacket.push_back(timeDelta(start2, end2));

            }

            if (!decoder.canDecodeGeneration(i)) {
                goto err;
            }

            if (gettimeofday(&start1, NULL)) {
                goto err;
            }

            if (!decoder.decodeGeneration(i)) {
                goto err;
            }

            if (gettimeofday(&end1, NULL)) {
                goto err;
            }

            decodeTimePerGeneration.push_back(timeDelta(start1, end1));
            decodeTimePerFile[k] += timeDelta(start1, end1);

        }

        if (gettimeofday(&start1, NULL)) {
            goto err;
        }

        decoder.flush();

        if (gettimeofday(&end1, NULL)) {
            goto err;
        }

        timePerFlush.push_back(timeDelta(start1, end1));
        decodeTimePerFile[k] += timeDelta(start1, end1);

        if (gettimeofday(&end3, NULL)) {
            goto err;
        }

        totalTime.push_back(timeDelta(start3, end3));

    }

    averageEncodePerPacket = accumulate(encodeTimePerPacket.begin(), encodeTimePerPacket.end(), 0) / encodeTimePerPacket.size();
    averageEncodePerGeneration = accumulate(encodeTimePerGeneration.begin(), encodeTimePerGeneration.end(), 0) / encodeTimePerGeneration.size();
    averageDecodePerPacket = accumulate(decodeTimePerPacket.begin(), decodeTimePerPacket.end(), 0) / decodeTimePerPacket.size();
    averageDecodePerGeneration = accumulate(decodeTimePerGeneration.begin(), decodeTimePerGeneration.end(), 0) / decodeTimePerGeneration.size();
    averageDecodePerFile = accumulate(decodeTimePerFile.begin(), decodeTimePerFile.end(), 0) / decodeTimePerFile.size();
    averageFlush = accumulate(timePerFlush.begin(), timePerFlush.end(), 0) / timePerFlush.size();
    averageTime = accumulate(totalTime.begin(), totalTime.end(), 0) / totalTime.size();

    minEncodePerPacket = *min_element(encodeTimePerPacket.begin(), encodeTimePerPacket.end());
    minEncodePerGeneration = *min_element(encodeTimePerGeneration.begin(), encodeTimePerGeneration.end());
    minDecodePerPacket = *min_element(decodeTimePerPacket.begin(), decodeTimePerPacket.end());
    minDecodePerGeneration = *min_element(decodeTimePerGeneration.begin(), decodeTimePerGeneration.end());
    minDecodePerFile = *min_element(decodeTimePerFile.begin(), decodeTimePerFile.end());
    minFlush = *min_element(timePerFlush.begin(), timePerFlush.end());
    minTime = *min_element(totalTime.begin(), totalTime.end());

    maxEncodePerPacket = *max_element(encodeTimePerPacket.begin(), encodeTimePerPacket.end());
    maxEncodePerGeneration = *max_element(encodeTimePerGeneration.begin(), encodeTimePerGeneration.end());
    maxDecodePerPacket = *max_element(decodeTimePerPacket.begin(), decodeTimePerPacket.end());
    maxDecodePerGeneration = *max_element(decodeTimePerGeneration.begin(), decodeTimePerGeneration.end()); 
    maxDecodePerFile = *max_element(decodeTimePerFile.begin(), decodeTimePerFile.end()); 
    maxFlush = *max_element(timePerFlush.begin(), timePerFlush.end());
    maxTime = *max_element(totalTime.begin(), totalTime.end()); 

    printf("%s(%lu, %lu, %lu) - %lu G: TT %lu (%lu -> %lu), EP %lu (%lu -> %lu), EG %lu (%lu -> %lu), DP %lu (%lu -> %lu), DG %lu (%lu -> %lu), DF %lu (%lu -> %lu), FL %lu (%lu -> %lu)\n", name, blockSize, blocksPerGeneration, dataLength, numGenerations, averageTime, minTime, maxTime, averageEncodePerPacket, minEncodePerPacket, maxEncodePerPacket, averageEncodePerGeneration, minEncodePerGeneration, maxEncodePerGeneration, averageDecodePerPacket, minDecodePerPacket, maxDecodePerPacket, averageDecodePerGeneration, minDecodePerGeneration, maxDecodePerGeneration, averageDecodePerFile, minDecodePerFile, maxDecodePerFile, averageFlush, minFlush, maxFlush);

err:
    freeBuffers(buffers);
    delete [] data;
    remove("test.enc");
    remove("test.dec");
}

struct MultiTestCase {
    size_t blockSize;
    size_t blocksPerGeneration;
    size_t dataLength;
    size_t numIterations;
};

template <typename B> void benchCoderMulti(vector<MultiTestCase> cases, const char *name)
{

    for (size_t i = 0; i < cases.size(); i++) {
        benchCoder<B>(name, cases[i].blockSize, cases[i].blocksPerGeneration, cases[i].dataLength, cases[i].numIterations);
    }
}

int main() {

    srand(15);

    vector<MultiTestCase> cases = {
        {1, 4, 39, 10},
        {2048, 4, 43151, 10},
        {32, 16, 65537, 10},
        {32768, 16, 4*1048576, 10},
        {32768, 64, 1048576, 10},
    };

    benchCoderMulti<BlockyCoderMemory>(cases, "BlockyCoderMemory");
    benchCoderMulti<BlockyCoderFile>(cases, "BlockyCoderFile");
    benchCoderMulti<BlockyCoderMmap>(cases, "BlockyCoderMmap");

}
