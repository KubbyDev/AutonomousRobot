#include "Vector.h"

#include <stdlib.h>
#include <math.h>

//A very simple 2D vector class

Vector* newVector(float x, float y) {
    Vector* vec = (Vector*) malloc(sizeof(float)*2);
    vec->x = x;
    vec->y = y;
    return vec;
}

void vectorSet(Vector* vec, float x, float y) {
    vec->x = x;
    vec->y = y;
}

void vectorAdd(Vector* destination, Vector* other) {
    destination->x += other->x;
    destination->y += other->y;
}

void vectorMult(Vector* destination, float k) {
    destination->x *= k;
    destination->y *= k;
}

Vector* vectorCopy(Vector* source) {
    return newVector(source->x, source->y);
}

Vector* vectorfromRot(float angle) {
    return newVector(cos(angle), sin(angle));
}
