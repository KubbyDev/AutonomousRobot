#ifndef BOOLEANMATRIX_H
#define BOOLEANMATRIX_H

typedef struct BooleanMatrix BooleanMatrix;

BooleanMatrix* bm_new(unsigned int sizeX, unsigned int sizeY);
void bm_destroy(BooleanMatrix* matrix);

// Getters
int bm_get(BooleanMatrix* matrix, unsigned int x, unsigned int y);
unsigned int bm_sizeX(BooleanMatrix* matrix);
unsigned int bm_sizeY(BooleanMatrix* matrix);
unsigned char bm_getByte(BooleanMatrix* matrix, unsigned int i);
int bm_inBounds(BooleanMatrix* matrix, unsigned int x, unsigned int y);

//Setters
void bm_set(BooleanMatrix* matrix, unsigned int x, unsigned int y, int booleanValue);
void bm_clear(BooleanMatrix* matrix);
void bm_fill(BooleanMatrix* matrix, int booleanValue);

#endif //BOOLEANMATRIX_H
