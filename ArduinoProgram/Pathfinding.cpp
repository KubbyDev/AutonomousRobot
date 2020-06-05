#include "Pathfinding.h"
#include "Data.h"

#include <stdlib.h>

void findPath() {

    // Resets the pixels that are not walls in lowResMap (sets them to 254)
    for(int y = 0; y < LOWRES_SIZE; y++)
        for(int x = 0; x < LOWRES_SIZE; x++)
            if(getMatrixValue(lowResMap, x, y) != 255)
                setMatrixValue(lowResMap, x, y, 254);

    
}

void old_findPath() {

    //Serial.print("New path calculation. ");
    //unsigned long start = millis();

    // This algorithm is not optimal (really not)
    // But it uses only n integers, n being the number of pixels of the lowresmap
    // So it fits my usage because I don't have much memory to work with
    // (Something like Dijkstra would require a structure that can store n^2 numbers)

    // In short it goes through the map and for each pixel, if a neighbour pixel has a distance value
    // that is less or equal to its own distance value, it takes the neighbours pixel's value+1. 
    // And the algorithm does it repeatedly until there is no more change. The worst case scenario's
    // complexity is n^2, but in practise it is often much faster because a lot of pixels are updated
    // on each iteration.

    // Resets the pixels that are not walls in lowResMap (sets them to 254)
    for(int y = 0; y < LOWRES_SIZE; y++)
        for(int x = 0; x < LOWRES_SIZE; x++)
            if(getMatrixValue(lowResMap, x, y) != 255)
                setMatrixValue(lowResMap, x, y, 254);

    // Sets the targets distance to the target to 0
    Vector* targetPosition = vectorCopy(target);
    vectorMult(targetPosition, (float)LOWRES_SIZE/MAP_SIZE);
    setMatrixValue(lowResMap, targetPosition->x, targetPosition->y, 0);
    free(targetPosition);

    // List of all movements that are considered possible for the pathfinding
    char offsets[] = {1,0, 0,1, -1,0, 0,-1, 1,1, -1,1, -1,-1, 1,-1};

    // List of numbers for the update order
    // There are 4 update orders (top left to bottom right, top right to bottom left, bl to tr, br to tl)
    // For each order, there are startX, startY, stepX, stepY, endX, endY values
    char orders[] = {
        0, 0, 1, 1, LOWRES_SIZE, LOWRES_SIZE,
        LOWRES_SIZE-1, 0, -1, 1, -1, LOWRES_SIZE,
        0, LOWRES_SIZE-1, 1, -1, LOWRES_SIZE, -1,
        LOWRES_SIZE-1, LOWRES_SIZE-1, -1, -1, -1, -1
    };
    int selected = 0;
    
    // For all pixels in the map, calculates the distance to the target
    // Continues the calculation until there are pixels that have not been calculated
    int changed = 1;
    while(changed) {

        changed = 0;
        // The order of the update changes at each update (see the order array above)
        for(int y = orders[selected*6+1]; y != orders[selected*6+5]; y += orders[selected*6+3]) {
            for(int x = orders[selected*6+0]; x != orders[selected*6+4]; x += orders[selected*6+2]) {

                unsigned char current = getMatrixValue(lowResMap, x, y);
                if(current == 255)
                    continue;
                // For all pixels around this pixel
                for(int i = 0; i < 8; i++) {
                    int newX = x + offsets[2*i];
                    int newY = y + offsets[2*i +1];
                    if(inMatrixBounds(lowResMap, newX, newY) && getMatrixValue(lowResMap, newX, newY)+1 < current) {
                        setMatrixValue(lowResMap, x, y, getMatrixValue(lowResMap, newX, newY)+1);
                        changed = 1;
                    }
                }
            }
        }

        // Changes the order of the update (look at the order array above)
        selected = (selected+1) %4;
    }

    needsPathUpdate = 0;

    //Serial.print("Took ");Serial.print(millis() - start);Serial.println(" ms");
}
