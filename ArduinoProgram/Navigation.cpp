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

    // Calculates the needed turn input to reach the target
    float targetAngle = atan2(targetPos->y - position->y, 
                              targetPos->x - position->x);
    clampAngle(&targetAngle); // Ensures that the angle is in [0, 2PI]
    
    float angleDiff = targetAngle - rotation;
    //Avoids making a turn of more than 180 degrees
    if(targetAngle + PI < rotation) angleDiff += 2*PI;
    if(targetAngle - PI > rotation) angleDiff -= 2*PI;

    //Serial.print("rotation: ");Serial.print(rotation);Serial.print(" target: ");Serial.println(targetAngle);

    // Determines if the robot should turn or go forward
    // If it is already going forward, it will turn only if the 
    // angle difference is above a certain threshold
    // If it is already turning, it will continue turning until 
    // the target angle is reached
    if((newTurnInput == 0 && fabsf(angleDiff) > MIN_ANGLE)
    || (sign(newTurnInput == sign(angleDiff)))) 
    {
        newForwardInput = 0;
        newTurnInput = (angleDiff > 0) ? 1 : -1;
    }
    // If no turn is needed, can go forward
    else {
        newForwardInput = 1;
        newTurnInput = 0;
    }
}

//Gets the position next "checkpoint" to get to the target
Vector* getNextPosition() {

    // List of all possible movements
    // Diagonals are at the end so straight lines are prefered
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

    DBG Serial.println("Updating navigation ----------------------");

    // Checks if the robot is already on its target
    int alreadyOnTarget = vectorDistSqr(target, position) <= ((TARGET_SIZE*TARGET_SIZE)/(PIXEL_LENGTH*PIXEL_LENGTH));
    //Serial.print("Distance: ");Serial.println(vectorDistSqr(target, position));

    // If it is not, calculates the new movement inputs
    if( ! alreadyOnTarget) { 
 
        // If the robot is inside a wall, goes backwards
        if(getMatrixValue(lowResMap, round(position->x/3), round(position->y/3)) == 255) {
            DBG Serial.println("Inside a wall");
            newForwardInput = -1;
            newTurnInput = 0;
        }
        else {
            
            // Calculates the newTurnInput and the newForwardInput to go to targetPosition
            Vector* targetPosition = getNextPosition();    
            if(targetPosition != NULL)
                getMovementInput(targetPosition);    

            DBG Serial.print("Movement input: forward: ");Serial.print(newForwardInput);Serial.print(" turn: ");Serial.println(newTurnInput);
        
            free(targetPosition);
        }
    }
    else {
        DBG Serial.println("Already on target");
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
            resetLastUpdateTime();
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
    if(needsPathUpdate && !alreadyOnTarget) {
        long strt = millis();
        DBG Serial.println("Updating path");
        findPath();
        DBG Serial.print("Done ! ");Serial.print(millis() - strt);Serial.println("ms");            
    }

    // Updates the internMap and the lowResMap according to the data of the sonar
    // Updates needsPathUpdate if necessary
    DBG Serial.println("Updating Intern map");
    updateInternMap();
}
