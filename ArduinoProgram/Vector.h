#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} Vector;

Vector* newVector(float x, float y);
void vectorSet(Vector* vec, float x, float y);
void vectorAdd(Vector* destination, Vector* other);
void vectorMult(Vector* destination, float k);
Vector* vectorCopy(Vector* source);
Vector* vectorfromRot(float angle);

#endif //VECTOR_H
