#ifndef _PEBBLE_GLOBALS_H_
#define _PEBBLE_GLOBALS_H_

/*Typedefs for 2 byte and single byte information*/
/*!\typedef  
 * Typedef for 16bit unsigned short.
 */
typedef unsigned short uint16;

/*!\typedef  
 * Typedef for 8bit unsigned char.
 */
typedef unsigned char uchar;

/*! 
 *Define the total number of involved nodes. In future releases this will be a 
 * dynamic variable. \TODO: Change it to a variable instead of a define
 */
#define TOT_NUM_NODES 4

/**
 * Inline function to get the index of the (i,j) element in the array from the matrix representation.
 * @param i I-th index
 * @param j J-th index
 * @return The value of the index in the array
 */
static inline uint16 mat2vec(uchar i, uchar j) {
    uint16 res;
    res = i * TOT_NUM_NODES + j;
    return res;
}

#endif
