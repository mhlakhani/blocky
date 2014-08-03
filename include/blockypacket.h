/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _BLOCKYPACKET_H
#define _BLOCKYPACKET_H

namespace blocky {

struct BlockyPacket {
    size_t generation;
    size_t numBlocks;
    size_t blockSize;
    uint8_t *data;
    uint8_t *coeffs;

    BlockyPacket() :
        generation(0),
        numBlocks(0),
        blockSize(0),
        data(NULL),
        coeffs(NULL)
    {

    }
};

}

#endif
