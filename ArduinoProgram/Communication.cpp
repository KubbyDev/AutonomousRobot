#include "Data.h"
#include "Communication.h"

#include <Arduino.h>
#include <stdlib.h>

#define SERIALOBJECT Serial3 //The serial object used to communicate with the ESP8266

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

void readingError() {
    Serial.println("Error reading int from serial");
    while(SERIALOBJECT.available())
        SERIALOBJECT.read();
}

// Sends a chunk of the intern map to the ESP8266
// A chunk contains the data for lines 3*index to 3*(index+1)-1
void sendMapChunkToEsp(int chunkIndex) {

    char* chunk = (char*) malloc(sizeof(char)*3*9*3 +1);
    int firstByte = chunkIndex*9*3;
    for(int i = 0; i < 3*9; i++) {
        unsigned char uc = bm_getByte(internMap, i + firstByte);
        //Serial.print(uc);
        chunk[i*3 +0] = '0' + uc/100;
        chunk[i*3 +1] = '0' + (uc%100)/10;
        chunk[i*3 +2] = '0' + uc%10;
    }

    chunk[3*9*3] = '\n';
    SERIALOBJECT.write(chunk, 3*9*3+1);
    free(chunk);
}

// Sends the position and the rotation of the robot to the ESP8266
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
    //Serial.print("Got a message");Serial.println(commandType);
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
        //Serial.print("New Target: ");Serial.print(target->x);Serial.print(", ");Serial.print(target->y);Serial.println();
    }
    // If a map chunk is requested, sends it
    // Map chunk requests are of this form: M<CHUNKINDEX>\n
    if(commandType == 'M')
        sendMapChunkToEsp(readIntFromSerial('\n'));
    // If the position is requested, sends it
    // Position requests are of this form: P\n
    if(commandType == 'P') {
        sendPositionToEsp();
        SERIALOBJECT.read(); // Consumes the '\n'    
    }
}
