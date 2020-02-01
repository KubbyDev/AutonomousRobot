// ----------------------------------------------------------------------------------------------------------
//
// Program of the Arduino board. Functionnalities:
// - Navigates to the target position. The low res map is used to find a path to it
// - Makes sonar measurements and updates the intern map and the low res map
// - Answers the requests of the ESP via Serial (give the intern map or the position, change the target position)
// - Manages the servo for the arms
//
// ----------------------------------------------------------------------------------------------------------

#include "Data.h"

#include "Navigation.h"

#include "Motors.h"
#include "Sonar.h"
#include "Servo.h"

#include "Communication.h"

#include "BooleanMatrix.h"
#include "UCharMatrix.h"
#include "Vector.h"

void setup() {
  
    // Empties the internMap and the low resolution map
    bm_clear(internMap);
    clearMatrix(lowResMap);
    vectorSet(position, MAP_SIZE/2, MAP_SIZE/2);
    vectorSet(target, MAP_SIZE/2, MAP_SIZE/2);

    // Initialises the motors and the sonar
    initMotors();
    initSonar();

    // Initialises the communication with the ESP8266
    initCommunication();

    // Initialises the servo for the arms
    initServo();
}

void loop() {

    // Updates the communication with the ESP8266
    // (Retrives or sends information to the user if necessary)
    updateCommunication();

    // Updates the control program and the motors
    // (Changes the values in Data.cpp)
    updateNavigation();

    // Lauches a new sonar measurement
    updateSonar();

    // Updates the arms position
    updateServo();
}
