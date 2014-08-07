/*!
    @file
    @brief Galois Field Operations
    @author Hasnain Lakhani
    @date 2014
    @copyright (c) 2014, see LICENSE for details
*/

#ifndef _GF28_H
#define _GF28_H

#include <cstdlib>
#include <cstdint>

namespace blocky {

/*! @brief Galos Field Operations

    Operations are in GF(2^8) with 3 as the generator
*/
class GF28 {

public:

    /*! @brief Constructor */
    GF28() {}

    /*! @brief Destructor */
    ~GF28() {}

    /*! @brief Adds two field elements
        @param[in] x A field element
        @param[in] y A field element
        @return x+y
    */
    inline uint8_t add(uint8_t x, uint8_t y) 
    {
        return x ^ y;
    }

    /*! @brief Subtracts two field elements
        @param[in] x A field element
        @param[in] y A field element
        @return x-y
    */
    inline uint8_t sub(uint8_t x, uint8_t y) 
    {
        return x ^ y;
    }

    /*! @brief Multiplies two field elements
        @param[in] x A field element
        @param[in] y A field element
        @return x*y
    */
    inline uint8_t mul(uint8_t x, uint8_t y) 
    {
        if (x == 0 || y == 0) {
            return 0;
        } else {
            return AL[L[x] + L[y]];
        }
    }

    /*! @brief Divides two field elements
        @param[in] x A field element
        @param[in] y A field element
        @return x/y
    */
    inline uint8_t div(uint8_t x, uint8_t y) 
    {
        if (x == 0 || y == 0) {
            return 0;
        } else {
            return AL[L[x] - L[y] + 255];
        }
    }


    /*! @brief Adds a constant to an array of field elements
        @param[in] x A field element
        @param[in,out] data The array of field elements (will be updated in place)
        @param[in] size The size of the array
    */
    inline void add(uint8_t x, uint8_t *data, size_t size) 
    {

        if (x == 0) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = add(data[i], x);
        }
    }

    /*! @brief Subtracts a constant from an array of field elements
        @param[in] x A field element
        @param[in,out] data The array of field elements (will be updated in place)
        @param[in] size The size of the array
    */
    inline void sub(uint8_t x, uint8_t *data, size_t size) 
    {

        if (x == 0) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = sub(data[i], x);
        }
    }

    /*! @brief Multiplies an array of field elements by a constant
        @param[in] x A field element
        @param[in,out] data The array of field elements (will be updated in place)
        @param[in] size The size of the array
    */
    inline void mul(uint8_t x, uint8_t *data, size_t size) 
    {

        if (x == 1) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = mul(data[i], x);
        }
    }

    /*! @brief Divides an array of field elements by a constant
        @param[in] x A field element
        @param[in,out] data The array of field elements (will be updated in place)
        @param[in] size The size of the array
    */
    inline void div(uint8_t x, uint8_t *data, size_t size) {

        if (x == 1) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data[i] = div(data[i], x);
        }
    }


    /*! @brief Adds a linear multiple of an array of field elements to another one
        @param[in] c The constant of multiplication
        @param[in,out] data1 The base array (will be updated in place)
        @param[in] data2 The array to add multiples of
        @param[in] size The size of the arrays

        Performs the operation \f$data1 = data1 + c \cdot data2\f$
    */
    inline void addMultiple(uint8_t c, uint8_t *data1, uint8_t *data2, size_t size) 
    {

        if (c == 0) {
            return;
        }

        for (size_t i = 0; i < size; i++) {
            data1[i] = add(data1[i], mul(data2[i], c));
        }
    }

    /*! @brief Subtracts a linear multiple of an array of field elements from another one
        @param[in] c The constant of multiplication
        @param[in,out] data1 The base array (will be updated in place)
        @param[in] data2 The array to subtract multiples of
        @param[in] size The size of the arrays

        Performs the operation \f$data1 = data1 - c \cdot data2\f$
    */
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

    /*! @brief Table of antilogs */
    static const uint8_t AL[512]; // antilogs

    /*! @brief Table of logs */
    static const uint8_t L[256]; // logs
};

}

#endif
