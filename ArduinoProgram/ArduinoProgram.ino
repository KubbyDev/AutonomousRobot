// ----------------------------------------------------------------------------------------------------------
//
// Program of the Arduino board. Functionnalities:
// - Navigates to the target position. The low res map is used to find a path to it
// - Makes sonar measurements and updates the intern map and the low res map
// - Answers the requests of the ESP via Serial (give the intern map or the position, change the target position)
//
// ----------------------------------------------------------------------------------------------------------

#include "Data.h"

#include "Navigation.h"

#include "Motors.h"
#include "Sonar.h"

#include "Communication.h"

#include "BooleanMatrix.h"
#include "UCharMatrix.h"
#include "Vector.h"

void setup() {
  
    // Empties the internMap and the low resolution map
    bm_clear(internMap);
    clearMatrix(lowResMap);
    vectorSet(position, 36, 36);
    vectorSet(target, 36, 36);

    // Initialises the motors and the sonar
    initMotors();
    initSonar();

    // Initialises the communication with the ESP8266
    initCommunication();
}

void loop() {

    // Updates the communication with the ESP8266
    // (Retrives or sends information to the user if necessary)
    updateCommunication();
    
    // Updates the control algorithm
    // (Changes the values in Data.cpp)
    updateNavigation();
    
    // Updates the speed of the motors
    updateMotors();

    // Lauches a new sonar measurement
    updateSonar();
}
