#include "BooleanMatrix.h"
#include "UCharMatrix.h"
#include "Vector.h"
#include "Data.h"

// Control algorithm data ------------------------------------------------------

// Intern map
BooleanMatrix* internMap = bm_new(MAP_SIZE, MAP_SIZE);

// Low resolution map (Intern map but with a smaller resolution). A pixel in the
// lowResMap is ON if at least 1 pixel of the intern map that touches it is ON
UCharMatrix* lowResMap = newMatrix(LOWRES_SIZE, LOWRES_SIZE);

// Position and rotation (cm and radians)
Vector* position = newVector(MAP_SIZE/2,MAP_SIZE/2);
float rotation = 0;

// Sonar data (last distance measured in cm)
float lastDistance = 0;

// Last forward input (1 = full forward, -1 = full backward)
float forwardInput = 0;
// Last turn input (-1 = full right, 1 = full left)
float turnInput = 0;

// The position of the target
Vector* target = newVector(0,0);

// True if the path needs to be updated (the map/target just changed)
int needsPathUpdate = 1;

// Last navigation update time in micro seconds
unsigned long lastUpdateTime = 0;
