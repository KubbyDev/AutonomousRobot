#include "Navigation.h"
#include "Pathfinding.h"
#include "InternMap.h"
#include "Data.h"
#include "Vector.h"
#include "UCharMatrix.h"
#include "BooleanMatrix.h"
#include "Sonar.h"

#include <Arduino.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void clampAngle(float* angle) {
    *angle = fmodf(*angle, 2*PI);
    if(*angle < 0)
        *angle += (2*PI);
}

void getMovementInput(Vector* targetPos, float* forwardInput, float* turnInput) {

    *forwardInput = 0;
    *turnInput = 0;

    // Calculates the needed turn input to reach the target
    float targetAngle = atan2(targetPos->y - position->y, 
                              targetPos->x - position->x);

    /*
    Serial.println("-------------------------");
    Serial.print("PosX: ");Serial.println(position->x);    
    Serial.print("PosY: ");Serial.println(position->y);
    Serial.print("TargetX: ");Serial.println(target->x);    
    Serial.print("TargetY: ");Serial.println(target->y);
    Serial.print("DeltaX: ");Serial.println(targetPos->x - position->x);    
    Serial.print("DeltaY: ");Serial.println(targetPos->y - position->y);
    Serial.print("Current angle: ");Serial.println(rotation);
    Serial.print("Target angle: ");Serial.println(targetAngle);
    */
    
    clampAngle(&targetAngle);
    
    float angleDiff = fabsf(targetAngle - rotation);

    //If the angle difference is too small, doesn't turn
    if(angleDiff > 0.5f) {

        //Avoids making a turn of more than 180 degrees
        if(targetAngle - rotation > PI) targetAngle -= 2*PI;
        if(targetAngle - rotation < -PI) targetAngle += 2*PI;

        *turnInput = (targetAngle > rotation) ? 1 : -1;
    }
    //If no big turn is needed, can go forward
    else {
        *forwardInput = 1;
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
    
    //Gets the time between this update and the previous one (seconds)
    float deltaTime = (float) (micros() - lastUpdateTime) * 1e-6; 
    lastUpdateTime = micros();
    
    //Calculates the turnInput and the forwardInput to go to targetPosition
    Vector* targetPosition = getNextPosition();    
    if(targetPosition != NULL)
        getMovementInput(targetPosition, &forwardInput, &turnInput);    
    free(targetPosition);

    //Updates the position and rotation of the robot on the map
    float speed = forwardInput * robotSpeed/pixelLength * deltaTime;
    position->x += cos(rotation) * speed;
    position->y += sin(rotation) * speed;
    rotation += turnInput * robotTurnRate * deltaTime;
    clampAngle(&rotation);
    
    //Updates the path if necessary
    if(needsPathUpdate)
        findPath();

    //Updates the internMap and the lowResMap according to the data of the sonar
    //Updates needsPathUpdate if necessary
    updateInternMap();
}
