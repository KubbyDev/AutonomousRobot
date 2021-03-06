#ifndef DATA_H
#define DATA_H


// TEMPORARY



#define DBG Serial.print(millis()/1000);Serial.print(".");Serial.print(millis()%1000);Serial.print(" ");





#include "BooleanMatrix.h"
#include "UCharMatrix.h"
#include "Vector.h"

// Pins values --------------------------------------------------------------------------------------

// Motors (Powering: 12V)
#define IN1  4
#define IN2  5
#define IN3  6
#define IN4  7
#define ENA  9   // Needs to be a PWM pin (this one is a PWN on both the UNO and the MEGA)
#define ENB  10  // Needs to be a PWM pin (this one is a PWN on both the UNO and the MEGA)

// Sonar (Powering: 5V)
#define TRIGGER_PIN 11
#define ECHO_PIN 12  // Must be a PCINT pin (this one is a PCINT on both the UNO and the MEGA)

// Servo (Powering: 3.3V)
#define SERVO_PIN 2

// Experimentally determined values -----------------------------------------------------------------
// You should use Tests/SpeedCalc/SpeedCalc.ino to calculate these values

// Forward speed in cm/s (measure it with Tests/SpeedCalc/SpeedCalc.ino)
#define ROBOT_SPEED 65.0f
// Turn rate in rad/s (measure it with Tests/SpeedCalc/SpeedCalc.ino)
#define ROBOT_TURN_RATE 6.88f
// The speed of the motors. These values should be as low as possible to make 
// position tracking as accurate as possible. But if you make it too low, the motors 
// wont turn. Also you should try to tweak them in order to make the robot go in a
// straight line when it should (use SpeedCalc.ino to make these adjustments)
#define LEFT_MOTOR_SPEED 90
#define RIGHT_MOTOR_SPEED 97
// The time the robot takes to reach its max speed (microseconds)
#define ACCEL_DURATION 308000

// Control configuration values ---------------------------------------------------------------------

// Size of the main map (must be a multiple of 3)
// Check if you have enough memory before increasing it. 
// Change it in the smartphone app too if you change it here
#define MAP_SIZE 72
// Size of the pathfinding map (Don't change)
#define LOWRES_SIZE MAP_SIZE/3
// Real world length of a main map pixel in cm. If you increase it you increase the real world size of the map
#define PIXEL_LENGTH 3.0f
// When the robot changes direction, it will stop and wait WAIT_TIME before continuing (microseconds)
#define WAIT_TIME 200000
// The minimum difference between the target direction and the robot direction to make the robot turn (radians)
#define MIN_ANGLE 0.8f
// The distance the robot has to be from its target to consider it is reached (in centimeters)
#define TARGET_SIZE 5.0f
// The min and max angles for the arms (0 is horizontal)
#define SERVO_MIN 120
#define SERVO_MAX 180
// The time in milliseconds between two arms position changes
#define SERVO_PERIOD 300

// Global variables ---------------------------------------------------------------------------------

extern BooleanMatrix* internMap;
extern UCharMatrix* lowResMap;
extern Vector* position;
extern float rotation;
extern float lastDistance;
extern float forwardInput;
extern float turnInput;
extern Vector* target;
extern int needsPathUpdate;
extern unsigned long navStateChangeTime;

#endif //DATA_H
