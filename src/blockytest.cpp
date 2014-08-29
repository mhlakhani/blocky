/*!
    @file
    @brief BlockyTest
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
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;
using namespace blocky;

void freeBuffers(vector<uint8_t *> buffers) 
{

    for (size_t i = 0; i < buffers.size(); i++) {
        delete [] buffers[i];
    }

}

template <typename B> bool testEndToEndBlockyCoder(size_t blockSize, size_t blocksPerGeneration, size_t dataLength, bool verifyFileOutput = true) 
{

    uint8_t *data = new uint8_t[dataLength];
    for (size_t i = 0; i < dataLength; i++) {
        uint8_t v = i;
        data[i] = v;
    }

    ofstream encfile("test.enc", ios::out|ios::binary);
    if (!encfile.is_open()) {
        printf("ERROR: Error opening temp file\n");
        return false;
    }

    encfile.write((char *) data, dataLength);
    encfile.close();

    vector<uint8_t *> datas;
    vector<uint8_t *> coeffss;
    {
        B encoder = Utils::createBlockyEncoder<B>(blockSize, blocksPerGeneration, dataLength, "test.enc", data);
        B decoder = Utils::createBlockyDecoder<B>(blockSize, blocksPerGeneration, dataLength, "test.dec", data);

        for (size_t i = 0; i < encoder.getNumGenerations(); i++) {
            for (size_t j = 0; j < encoder.getNumBlocksInGeneration(i); j++) {

                BlockyPacket packet;

                if (!encoder.encode(packet, i)) {
                    printf("Error encoding packet (%lu, %lu)!\n", i, j);
                }
                datas.push_back(packet.data);
                coeffss.push_back(packet.coeffs);

                while (!decoder.store(packet)) {
                    encoder.encode(packet, i);
                    continue;
                }

                if (decoder.getRank(i) != j+1) {
                    printf("rank %lu != %lu!\n", decoder.getRank(i), j+1);
                    goto err;
                }

            }

            if (!decoder.canDecodeGeneration(i)) {
                printf("Can't decode generation %lu!\n", i);
            }
        }

        if (!decoder.canDecode()) {
            printf("Can't decode!\n");
            goto err;
        }

        if (!decoder.decode()) {
            printf("Decoding failed!\n");
            goto err;
        }

        if (!decoder.flush()) {
            printf("Flushing failed!\n");
            goto err;
        }

        for (size_t i = 0; i < dataLength; i++) {
            uint8_t v = i;
            if (decoder.getBuffer()[i] != v) {
                printf("buffer[%lu] = %u != %u!\n", i, decoder.getBuffer()[i], v);
                goto err;
            }
        }
    }

    if (verifyFileOutput)
    {
        ifstream decfile("test.dec", ios::in|ios::binary);
        if (!decfile.is_open()) {
            printf("ERROR: Error opening temp file\n");
            return false;
        }

        decfile.read((char *) data, dataLength);
        decfile.close();

        for (size_t i = 0; i < dataLength; i++) {
            uint8_t v = i;
            if (data[i] != v) {
                printf("data[%lu] = %u != %u!\n", i, data[i], v);
                goto err;
            }
        }
    }

    freeBuffers(datas);
    freeBuffers(coeffss);
    delete [] data;
    remove("test.enc");
    remove("test.dec");
    return true;

err:
    freeBuffers(datas);
    freeBuffers(coeffss);
    delete [] data;
    remove("test.enc");
    remove("test.dec");
    return false;
}

struct MultiTestCase {
    size_t blockSize;
    size_t blocksPerGeneration;
    size_t dataLength;
};

template <typename B> bool testEndToEndBlockyCoderMulti(vector<MultiTestCase> cases, const char *name, bool verifyFileOutput = true)
{

    bool retval = true;
    for (size_t i = 0; i < cases.size(); i++) {
        bool success = testEndToEndBlockyCoder<B>(cases[i].blockSize, cases[i].blocksPerGeneration, cases[i].dataLength, verifyFileOutput);
        printf("%s(%lu, %lu, %lu): %s\n", name, cases[i].blockSize, cases[i].blocksPerGeneration, cases[i].dataLength, success ? "true" : "false");
        retval &= success;
    }
    return retval;
}

int main() {

    srand(15);

    vector<MultiTestCase> cases = {
        {1, 4, 39},
        {2048, 4, 43151},
        {32, 16, 65537},
        {32768, 16, 4*1048576}
    };
    bool success = true;

    success &= testEndToEndBlockyCoderMulti<BlockyCoderMemory>(cases, "testEndToEndBlockyCoderMemory", false);
    success &= testEndToEndBlockyCoderMulti<BlockyCoderFile>(cases, "testEndToEndBlockyCoderFile");
    success &= testEndToEndBlockyCoderMulti<BlockyCoderMmap>(cases, "testEndToEndBlockyCoderMmap");

    if (success) {
        printf("All tests passed!\n");
    } else {
        printf("Some tests FAILED!\n");
    }

}
