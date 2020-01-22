#ifndef DATA_H
#define DATA_H

#include "BooleanMatrix.h"
#include "UCharMatrix.h"
#include "Vector.h"

// Size of the main map (must be a multiple of 3, better to also be a multiple of 8)
// Check if you have enough memory before increasing it. 
// Change it in the smartphone app too if you change it here
#define MAP_SIZE 72
// Size of the pathfinding map (Don't change)
#define LOWRES_SIZE MAP_SIZE/3
// Real world length of a main map pixel in cm. If you decrease it you increase the range of the map
#define pixelLength 10.0f
// Forward speed in cm/s (measure it with Tests/SpeedCalc/SpeedCalc.ino)
#define robotSpeed 55.0f
// Turn rate in rad/s (measure it with Tests/SpeedCalc/SpeedCalc.ino)
#define robotTurnRate 6.81f

// Motors (Powering: 12V)
#define IN1  2
#define IN2  3
#define IN3  5
#define IN4  6
#define ENA  9   // Needs to be a PWM pin (this one is a PWN on both the UNO and the MEGA)
#define ENB  10  // Needs to be a PWM pin (this one is a PWN on both the UNO and the MEGA)

// Sonar (Powering: 5V)
#define TRIGGER_PIN 11
#define ECHO_PIN 12  // Must be a PCINT pin (this one is a PCINT on both the UNO and the MEGA)

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
