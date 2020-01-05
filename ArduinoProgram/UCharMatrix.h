#ifndef UCHARMATRIX_H
#define UCHARMATRIX_H

typedef struct UCharMatrix UCharMatrix;

UCharMatrix* newMatrix(unsigned int sizeX, unsigned int sizeY);
void destroyMatrix(UCharMatrix* matrix);

unsigned char getMatrixValue(UCharMatrix* matrix, unsigned int x, unsigned int y);
int inMatrixBounds(UCharMatrix* matrix, unsigned int x, unsigned int y);

void setMatrixValue(UCharMatrix* matrix, unsigned int x, unsigned int y, unsigned char value);
void clearMatrix(UCharMatrix* matrix);
void fillMatrix(UCharMatrix* matrix, unsigned char value);

#endif //UCHARMATRIX_H