#include "Data.h"
#include "Communication.h"

#include <Arduino.h>
#include <stdlib.h>

//The serial object used to communicate with the ESP8266
//You might have to replace it by a SoftwareSerial if you use a separate ESP
#define SERIALOBJECT Serial3

// Helper functions ----------------------------------------------------------------------

// Reads Serial input until endCharacter is reached, 
// then returns the int represented by the read input
// Returns -1 if the number contains more than 3 digits (error)
int readIntFromSerial(char endCharacter) {
    char* res = (char*) malloc(sizeof(char)*10);
    char next = SERIALOBJECT.read();
    int index = 0;
    while(next != endCharacter) {
        if(index == 3)
            return -1;
        res[index] = next;
        index++;
        next = SERIALOBJECT.read();
    }
    res[index] = 0;
    int result = atoi(res);
    free(res);
    return result;
}

// Prints an error message and flushes the incomming serial data
void readingError() {
    Serial.println("Error reading int from serial");
    while(SERIALOBJECT.available())
        SERIALOBJECT.read();
}

// Converts an number from 0 to 15 to its hexadecimal representation
char toHex(unsigned char i) {
    return i < 10 ? i+'0' : i-10+'A';
}

// Requests handling ---------------------------------------------------------------------

// Sends the entire map to the ESP through Serial
// The ESP has to be reading the data while it's being written because the
// Serial buffer can only hold 256 bytes on the Mega and less on the Uno
void sendMapToEsp() {
    
    for(int i = 0; i < 9*72; i++) {
        
        // Gets the number from the map
        unsigned char uc = bm_getByte(internMap, i);
        
        // Transmits the number in hexadecimal form
        SERIALOBJECT.write(toHex(uc/16));
        SERIALOBJECT.write(toHex(uc%16));
        
        // Waits for the ESP to read the data
        if(i%16 == 0)
            SERIALOBJECT.flush();
    }
    SERIALOBJECT.write('\n');
}

// Sends the position and the rotation of the robot to the ESP8266
// The response looks like this: 0.000   ;0.000   ;0.000   
// TODO: Do something cleaner
void sendPositionToEsp() {

    char* response = (char*) malloc(sizeof(char)*(3*15 +3));
    char* buffer = response;

    dtostrf(position->x, 15, 3, buffer);
    buffer += 15;
    buffer[0] = ';';
    buffer++;
    dtostrf(position->y, 15, 3, buffer);
    buffer += 15;
    buffer[0] = ';';
    buffer++;
    dtostrf(rotation, 15, 3, buffer);
    buffer += 15;
    buffer[0] = '\n';
    buffer += 2;
    
    SERIALOBJECT.write(response, 3*15+3);
    free(response);
}

// Main functions ------------------------------------------------------------------------

void initCommunication() {
    SERIALOBJECT.begin(250000);
}

void updateCommunication() {

    // If no command is received, does nothing
    if(! SERIALOBJECT.available())
        return;

    // Waits a bit just to be sure that the data has been written entirely
    delay(10);

    char commandType = SERIALOBJECT.read();
    // If a new target position is received, updates it
    // Target position update commands are of this form: T<POSX>,<POSY>\n
    if(commandType == 'T') {
        int x = readIntFromSerial(',');
        int y = readIntFromSerial('\n');
        if(x == -1 || y == -1) {
            readingError();
            return;
        }
        vectorSet(target, x, y);
        needsPathUpdate = 1;
    }
    // If the map is requested, sends it
    // Map requests are of this form: M\n
    if(commandType == 'M') {
        sendMapToEsp();
        SERIALOBJECT.read(); // Consumes the '\n'    
    }
    // If the position is requested, sends it
    // Position requests are of this form: P\n
    if(commandType == 'P') {
        sendPositionToEsp();
        SERIALOBJECT.read(); // Consumes the '\n'    
    }
}
