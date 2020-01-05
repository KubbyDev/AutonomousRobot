#include "UCharMatrix.h"

#include <stdlib.h>

struct UCharMatrix {
    unsigned char* values;
    unsigned int sizeX;
    unsigned int sizeY;
};

/** Constructs a matrix of given size */
UCharMatrix* newMatrix(unsigned int sizeX, unsigned int sizeY) {

    UCharMatrix* matrix = (UCharMatrix*) malloc(sizeof(unsigned int)*2 + sizeof(unsigned char*));

    matrix->values = (unsigned char*) malloc(sizeof(unsigned char) * sizeX * sizeY);
    matrix->sizeX = sizeX;
    matrix->sizeY = sizeY;

    return matrix;
}

/** Frees all the memory used by the matrix */
void destroyMatrix(UCharMatrix* matrix) {
    free(matrix->values);
    free(matrix);
}

// Getters ---------------------------------------------------------------------

/** Returns the value at position (x, y) */
unsigned char getMatrixValue(UCharMatrix* matrix, unsigned int x, unsigned int y) {
    return matrix->values[y*matrix->sizeX + x];
}

/** Returns 1 if the given coordinates are in the matrix bounds, 0 otherwise */
int inMatrixBounds(UCharMatrix* matrix, unsigned int x, unsigned int y) {
    return x < matrix->sizeX && y < matrix->sizeY;
}

// Setters ---------------------------------------------------------------------

/** Sets the value at position (x, y) to the one in parameter */
void setMatrixValue(UCharMatrix* matrix, unsigned int x, unsigned int y, unsigned char value) {
    matrix->values[y*matrix->sizeX + x] = value;
}

/** Fills the matrix with 0 */
void clearMatrix(UCharMatrix* matrix) { fillMatrix(matrix, 0); }

/** Fills the matrix with the desired value */
void fillMatrix(UCharMatrix* matrix, unsigned char value) {
    unsigned int arraySize = (matrix->sizeX)*(matrix->sizeY);
    for(unsigned int i = 0; i < arraySize; i++)
        matrix->values[i] = value;
}
