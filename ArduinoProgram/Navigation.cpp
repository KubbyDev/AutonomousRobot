#include "Navigation.h"
#include "Pathfinding.h"
#include "InternMap.h"
#include "PositionTracker.h"
#include "Data.h"
#include "Vector.h"
#include "UCharMatrix.h"
#include "BooleanMatrix.h"
#include "Sonar.h"
#include "Motors.h"
#include "Tools.h"

#include <Arduino.h>

int waiting = 0; // When the robot changes its movement input, it will wait for WAIT_TIME before moving again
float newForwardInput = 0;
float newTurnInput = 0;

void getMovementInput(Vector* targetPos) {

    newForwardInput = 0;
    newTurnInput = 0;

    // Calculates the needed turn input to reach the target
    float targetAngle = atan2(targetPos->y - position->y, 
                              targetPos->x - position->x);
    
    clampAngle(&targetAngle);
    
    float angleDiff = fabsf(targetAngle - rotation);

    //If the angle difference is too small, doesn't turn
    if(angleDiff > 0.5f) {

        //Avoids making a turn of more than 180 degrees
        if(targetAngle - rotation > PI) targetAngle -= 2*PI;
        if(targetAngle - rotation < -PI) targetAngle += 2*PI;

        newTurnInput = (targetAngle > rotation) ? 1 : -1;
    }
    //If no big turn is needed, can go forward
    else {
        newForwardInput = 1;
    }
}

//Gets the position next "checkpoint" to get to the target
Vector* getNextPosition() {

    //List of all possible movements
    //Diagonals are at the end so straight lines are prefered
    int offsets[] = {1,0, 0,1, -1,0, 0,-1, 1,1, -1,1, -1,-1, 1,-1};

    //Gets the tile with the smallest distance from the target
    Vector* pos = newVector(roundf(position->x/3), roundf(position->y/3));
    int minIndex = 0;
    unsigned char min = 255;
    for(int i = 0; i < 8; i++) {
        int newX = pos->x + offsets[2*i];
        int newY = pos->y + offsets[2*i +1];
        if(inMatrixBounds(lowResMap, newX, newY) && (getMatrixValue(lowResMap, newX, newY) < min)) {
            min = getMatrixValue(lowResMap, newX, newY);
            minIndex = i;
        }
    }

    Vector* selectedOffset = newVector(offsets[2*minIndex], offsets[2*minIndex +1]);
    vectorAdd(pos, selectedOffset);
    vectorMult(pos, 3);
    free(selectedOffset);
    
    return pos;
}

void updateNavigation() {

    // Checks if the robot is already on its target
    int alreadyOnTarget = vectorDistSqr(target, position) <= 1;
    //Serial.print("Distance: ");Serial.println(vectorDistSqr(target, position));

    // If it is not, calculates the new movement inputs
    if( ! alreadyOnTarget) { 
        // Calculates the newTurnInput and the newForwardInput to go to targetPosition
        Vector* targetPosition = getNextPosition();    
        if(targetPosition != NULL)
            getMovementInput(targetPosition);    
        
        free(targetPosition);
    }
    else {
        newForwardInput = 0;
        newTurnInput = 0;
    }
    
    // Updates the position and rotation of the robot on the map
    // Also updates the motors when necessary
    // When the direction that the robot should follow changes, the robot
    // will stop for WAIT_TIME seconds and then start moving in the new direction
    if(waiting) {
        
        // Waiting state: checks if the waiting time is ended
        if(micros() - navStateChangeTime > WAIT_TIME) {
            forwardInput = newForwardInput;
            turnInput = newTurnInput;
            waiting = 0;
            navStateChangeTime = micros();
            updateMotors();
        }
    }
    else {
        
        // Normal state
        updatePosition();

        // New direction requested: switches to waiting state
        if(newForwardInput != forwardInput || newTurnInput != turnInput) {
        
            forwardInput = 0;
            turnInput = 0;
            navStateChangeTime = micros();
            updateMotors();
            waiting = 1;
        }
    }

    // Updates the path if necessary
    if(needsPathUpdate && !alreadyOnTarget)
        findPath();

    // Updates the internMap and the lowResMap according to the data of the sonar
    // Updates needsPathUpdate if necessary
    updateInternMap();
}
