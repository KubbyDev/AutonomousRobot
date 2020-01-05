#include "BooleanMatrix.h"

#include <stdlib.h>
#include <math.h>

/*
 * This class represents a boolean matrix
 * Each boolean uses only one bit of memory
 * The values are stored in a one line array of unsigned chars
 * containing 8 booleans each (1 in each bit)
 */

struct BooleanMatrix {
    unsigned char* values;  //The values: uchars 8 bools in each (1 in each bit)
    unsigned int sizeX;     //The size of the matrix (given at the construction)
    unsigned int sizeY;     //The size of the matrix (given at the construction)
    unsigned int bytesX;    //The allocated sizeX (in bytes)
};

/** Constructs a matrix containing sizeX * sizeY booleans */
BooleanMatrix* bm_new(unsigned int sizeX, unsigned int sizeY) {

    BooleanMatrix* matrix = (BooleanMatrix*) malloc(
            sizeof(BooleanMatrix));

    //The number of bytes to allocate for each line
    unsigned int bytesX = (unsigned int)ceilf((float)sizeX/8);

    matrix->values = (unsigned char*) malloc(
            sizeof(unsigned char) * bytesX * sizeY);
    matrix->sizeX = sizeX;
    matrix->sizeY = sizeY;
    matrix->bytesX = bytesX;

    return matrix;
}

/** Frees all the memory used by the matrix */
void bm_destroy(BooleanMatrix* matrix) {
    free(matrix->values);
    free(matrix);
}

// Getters ---------------------------------------------------------------------

/** Returns the value at position (x (horizontal), y (vertical)) */
int bm_get(BooleanMatrix* matrix, unsigned int x, unsigned int y) {

    //Gets the byte containing the desired boolean
    unsigned char holder = matrix->values[y*(matrix->bytesX) + x/8];

    //Isolates and returns the bit corresponding to the desired boolean
    unsigned char mask = 0b10000000;
    mask = mask >> (unsigned char) (x%8);
    unsigned char res = holder & mask;

    return res ? 1 : 0;
}

/** Returns the byte at position i in the buffer (x,y) = (i%width, i/width)
 * This byte encodes the values from i to i+7 included */
unsigned char bm_getByte(BooleanMatrix* matrix, unsigned int i) {
    return matrix->values[i];
}

/** Returns the number of columns of the matrix */
unsigned int bm_sizeX(BooleanMatrix* matrix) { return matrix->sizeX; }

/** Returns the number of lines of the matrix */
unsigned int bm_sizeY(BooleanMatrix* matrix) { return matrix->sizeY; }

/** Returns 1 if the given coordinates are in the matrix bounds, 0 otherwise */
int bm_inBounds(BooleanMatrix* matrix, unsigned int x, unsigned int y) {
    return x < matrix->sizeX && y < matrix->sizeY;
}

// Setters ---------------------------------------------------------------------

/** Sets the value at position (x, y) to the one in parameter */
void bm_set(BooleanMatrix* matrix, unsigned int x, unsigned int y, int booleanValue) {

    //Gets a pointer to the byte containing the desired boolean
    unsigned char* holder = &(matrix->values[y*(matrix->bytesX) + x/8]);

    //Creates a mask to target only the desired bit
    unsigned char mask = 0b10000000;
    mask = mask >> (unsigned char) (x%8);

    if(booleanValue) {
        //If the desired boolean is true, does a bitwise or
        //with 0s everywhere and 1 in the desired bit
        *holder = *holder | mask;
    }
    else {
        //If the desired boolean is false, does a bitwise and
        //with 1s everywhere and 0 in the desired bit
        mask = ~mask;
        *holder = *holder & mask;
    }
}

/** Fills the matrix with 0 (false) */
void bm_clear(BooleanMatrix* matrix) { bm_fill(matrix, 0); }

/** Fills the matrix with the desired value (0 -> false, != 0 -> true) */
void bm_fill(BooleanMatrix* matrix, int booleanValue) {

    unsigned char toFill = booleanValue ? 0xFF : 0x00;

    unsigned int arraySize = (matrix->bytesX)*(matrix->sizeY);
    for(unsigned int i = 0; i < arraySize; i++)
        matrix->values[i] = toFill;
}
