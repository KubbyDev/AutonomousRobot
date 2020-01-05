#include "InternMap.h"
#include "Data.h"
#include "Vector.h"
#include "BooleanMatrix.h"
#include "UCharMatrix.h"

#include <math.h>
#include <stdlib.h>

#include <Arduino.h>

// Turns a pixel ON on the intern map
// Also updates the lowResMap and controlAlgorithm.needsPathUpdate
void turnPixelOn(unsigned int x, unsigned int y) {

    //Stops everything if the pixel is outside the internMap or already ON
    if(!bm_inBounds(internMap, x, y) || bm_get(internMap, x, y))
        return;

    //Sets the pixel to ON
    bm_set(internMap, x, y, 1);

    // The new pixel will set a wall on every pixel on the lowResMap it touches
    for(int j = -1; j <= 1; j++) {
        for(int i = -1; i <= 1; i++) {
            int lrX = (x+i)/3;
            int lrY = (y+j)/3;
            if(inMatrixBounds(lowResMap, lrX, lrY) && (getMatrixValue(lowResMap, lrX, lrY) != 255)) {
                setMatrixValue(lowResMap, lrX, lrY, 255);
                needsPathUpdate = 1; //TODO: Seulement si le pixel etait sur le chemin
            }
        }
    }
}

// Checks if a lowResMap pixel should be off (if no real internMap pixel touches it)
int isPixelOff(unsigned int x, unsigned int y) {
    
    x *= 3;
    y *= 3;
    for(int j = -1; j <= 4; j++)
        for(int i = -1; i <= 4; i++)
            if(bm_inBounds(internMap, x+i, y+j) && bm_get(internMap, x+i, y+j))
                return 0;
    return 1;
}

// Turns a pixel OFF on the intern map
// Also updates the lowResMap and controlAlgorithm.needsPathUpdate
void turnPixelOff(unsigned int x, unsigned int y) {

    //Stops everything if the pixel is outside the internMap or already OFF
    if(!bm_inBounds(internMap, x, y) )//|| !bm_get(internMap, x, y))
        return;

    //Serial.print("Turning OFF ------");Serial.print(x);Serial.print(" ");Serial.println(y);

    //Sets the pixel to OFF
    bm_set(internMap, x, y, 0);

    //Creates an array of offsets for the pixels that can change
    int* toCheck = (int*) malloc(sizeof(int)*8);
    for(int i = 2; i < 8; i++)
        toCheck[i] = -10;
    //Adds the current pixel
    toCheck[0] = 0; toCheck[1] = 0;
    int nbToCheck = 1;

    //Adds the rest
    if(x % 3 == 0) { 
        toCheck[nbToCheck*2] = -1;
        toCheck[nbToCheck*2+1] = 0;
        nbToCheck++;
    }
    if(x % 3 == 2) { 
        toCheck[nbToCheck*2] = 1;
        toCheck[nbToCheck*2+1] = 0;
        nbToCheck++;
    }
    if(y % 3 == 0) { 
        toCheck[nbToCheck*2] = 0;
        toCheck[nbToCheck*2+1] = -1;
        nbToCheck++;
    }
    if(y % 3 == 2) { 
        toCheck[nbToCheck*2] = 0;
        toCheck[nbToCheck*2+1] = 1;
        nbToCheck++;
    }
    if((x % 3 == 0) && (y % 3 == 0)) { 
        toCheck[nbToCheck*2] = -1;
        toCheck[nbToCheck*2+1] = -1;
        nbToCheck++;
    }
    if((x % 3 == 2) && (y % 3 == 2)) { 
        toCheck[nbToCheck*2] = 1;
        toCheck[nbToCheck*2+1] = 1;
        nbToCheck++;
    }
    if((x % 3 == 0) && (y % 3 == 2)) { 
        toCheck[nbToCheck*2] = -1;
        toCheck[nbToCheck*2+1] = 1;
        nbToCheck++;
    }
    if((x % 3 == 2) && (y % 3 == 0)) { 
        toCheck[nbToCheck*2] = 1;
        toCheck[nbToCheck*2+1] = -1;
        nbToCheck++;
    }
    //Serial.println(nbToCheck);
    
    // Checks every pixel (if at least one pixel of the internMap touches the pixel,
    // it will stay black, otherwise it will be freed)
    int i = 0;
    while(i < 8 && toCheck[i] != -10) {

        //Serial.print("Checking ");Serial.print(i/2);Serial.print(", offset= ");Serial.print(toCheck[i]);Serial.print(" ");Serial.println(toCheck[i+1]);

        int lrX = x/3 + toCheck[i];
        int lrY = y/3 + toCheck[i+1];

        //Serial.print("Checking ");Serial.print(lrX);Serial.print(" ");Serial.println(lrY);

        // If the pixel is not in the matrix bounds or already off,
        // it will not have to be checked
        if(!inMatrixBounds(lowResMap, lrX, lrY) || (getMatrixValue(lowResMap, lrY, lrX) != 255)) {
            //Serial.print("not checked ");Serial.println(getMatrixValue(lowResMap, lrY, lrX));
            i += 2;
            continue;
        }

        if(isPixelOff(lrX, lrY)) {
            //Serial.print("SET OFF ");Serial.print(lrX);Serial.print(" ");Serial.println(lrY);
            setMatrixValue(lowResMap, lrX, lrY, 254);
            needsPathUpdate = 1;
        }
        //else Serial.println("not set off");

        i += 2;
    }

    free(toCheck);
}

//Updates the internMap according to the position, the rotation
//and the last distance calculated
void updateInternMap() {

    Vector* wallDir = vectorfromRot(rotation);
    float hitDistance = lastDistance / pixelLength; //In internMap units

    Vector* currentPixel = vectorCopy(position);

    //Serial.print("Position: x=");Serial.print(currentPixel->x);Serial.print(",y=");Serial.println(currentPixel->y);

    //Empties all the pixels between the robot and the hit point
    for(int i = 0; i <= hitDistance; i++) {
        //Serial.print("Current: x=");Serial.print(currentPixel->x);Serial.print(",y=");Serial.println(currentPixel->y);
        turnPixelOff(round(currentPixel->x), round(currentPixel->y));
        vectorAdd(currentPixel, wallDir);
    }

    //Fills the pixel at the hit point
    vectorSet(currentPixel, position->x, position->y);
    vectorMult(wallDir, hitDistance);
    vectorAdd(currentPixel, wallDir);
    turnPixelOn(round(currentPixel->x), round(currentPixel->y));
    //Serial.print("Hit: x=");Serial.print(currentPixel->x);Serial.print(",y=");Serial.println(currentPixel->y);

    free(wallDir);
    free(currentPixel);
}
