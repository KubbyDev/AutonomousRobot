#include "Pathfinding.h"
#include "Data.h"

#include <stdlib.h>

void findPath() {

    // This algorithm is not optimal (really not)
    // But it uses only n integers, n being the number of pixels 
    // of the lowresmap. So it fits my usage

    //Resets the pixels that are not walls in lowResMap (sets them to 254)
    for(int y = 0; y < LOWRES_SIZE; y++)
        for(int x = 0; x < LOWRES_SIZE; x++)
            if(getMatrixValue(lowResMap, x, y) != 255)
                setMatrixValue(lowResMap, x, y, 254);

    //Sets the targets distance to the target to 0
    Vector* targetPosition = vectorCopy(target);
    vectorMult(targetPosition, (float)LOWRES_SIZE/MAP_SIZE);
    setMatrixValue(lowResMap, targetPosition->x, targetPosition->y, 0);
    free(targetPosition);

    //List of all possible movements
    int offsets[] = {1,0, 0,1, -1,0, 0,-1, 1,1, -1,1, -1,-1, 1,-1};

    //Calculates the distances to the target
    int changed = 1;
    while(changed) {

        changed = 0;
        for(int y = 0; y < LOWRES_SIZE; y++) {
            for(int x = 0; x < LOWRES_SIZE; x++) {
                if(getMatrixValue(lowResMap, x, y) == 255)
                    continue;
                for(int i = 0; i < 8; i++) {
                    int newX = x + offsets[2*i];
                    int newY = y + offsets[2*i +1];
                    if(inMatrixBounds(lowResMap, newX, newY) 
                    && getMatrixValue(lowResMap, newX, newY)+1 < getMatrixValue(lowResMap, x, y)) {
                        setMatrixValue(lowResMap, x, y, getMatrixValue(lowResMap, newX, newY)+1);
                        changed = 1;
                    }
                }
            }
        }
    }

    needsPathUpdate = 0;
}
