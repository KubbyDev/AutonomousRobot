#ifndef DATA_H
#define DATA_H

#include "BooleanMatrix.h"
#include "UCharMatrix.h"
#include "Vector.h"

#define MAP_SIZE 72          // Size of the main map
#define LOWRES_SIZE 24       // Size of the pathfinding map
#define pixelLength 3.0f     // Length of a pixel in cm
#define robotSpeed 20.0f     // Forward speed in cm/s
#define robotTurnRate 0.8f   // Turn rate in rad/s

extern BooleanMatrix* internMap;
extern UCharMatrix* lowResMap;
extern Vector* position;
extern float rotation;
extern float lastDistance;
extern float forwardInput;
extern float turnInput;
extern Vector* target;
extern int needsPathUpdate;
extern unsigned long lastUpdateTime;

#endif //DATA_H
