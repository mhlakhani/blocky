/* Copyright (c) 2014, Hasnain Lakhani.
** All rights reserved.
** Please see LICENSE for details.
*/

#ifndef _GF28_H
#define _GF28_H

#include <cstdlib>
#include <cstdint>

namespace blocky {

class GF28 {

public:

    GF28() {}
    ~GF28() {}

    inline uint8_t add(uint8_t x, uint8_t y) 
    {
        return x ^ y;
    }

    inline uint8_t sub(uint8_t x, uint8_t y) 
    {
        return x ^ y;
    }

    inline uint8_t mul(uint8_t x, uint8_t y) 
    {
        if (x == 0 || y == 0) {
            return 0;
        } else {
            return AL[L[x] + L[y]];
        }
    }

    inline uint8_t div(uint8_t x, uint8_t y) 
    {
        if (x == 0 || y == 0) {
            return 0;
        } else {
            return AL[L[x] - L[y] + 255];
        }
    }


    inline void add(uint8_t x, uint8_t *data, size_t size) 
    {

        if (x == 0) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = add(data[i], x);
        }
    }

    inline void sub(uint8_t x, uint8_t *data, size_t size) 
    {

        if (x == 0) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = sub(data[i], x);
        }
    }

    inline void mul(uint8_t x, uint8_t *data, size_t size) 
    {

        if (x == 1) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = mul(data[i], x);
        }
    }

    inline void div(uint8_t x, uint8_t *data, size_t size) {

        if (x == 1) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = div(data[i], x);
        }
    }


    inline void addMultiple(uint8_t c, uint8_t *data1, uint8_t *data2, size_t size) 
    {

        if (c == 0) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data1[i] = add(data1[i], mul(data2[i], c));
        }
    }

    inline void subMultiple(uint8_t c, uint8_t *data1, uint8_t *data2, size_t size) 
    {

        if (c == 0) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data1[i] = sub(data1[i], mul(data2[i], c));
        }
    }

private:

    // 3 is our generator
    static const uint8_t AL[512]; // antilogs
    static const uint8_t L[256]; // logs
};

}

#endif
