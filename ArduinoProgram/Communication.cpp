#include "Data.h"
#include "Communication.h"

#include <Arduino.h>
#include <stdlib.h>

// The serial object used to communicate with the ESP8266
// You might have to replace it by a SoftwareSerial if you use a separate ESP
#define SERIALOBJECT Serial3

// The maximum time between the moment when a character is received 
// and the moment when the end of the message is received
#define TIMEOUT 100

// Helper functions ----------------------------------------------------------------------

// Reads an int from the str and returns it
// str gets automaticaly moved to the end of the str +1 char
// ex: "12 78" -> "78"
int readInt(char** str, char endCharacter) {

    char* s = *str;

    //Puts a null terminator at the end so the atoi function works
    int separatorPos = 0;
    while(s[separatorPos] != 0 && s[separatorPos] != endCharacter)
        separatorPos++;
    char separator = s[separatorPos];
    s[separatorPos] = 0;

    int res = atoi(s);

    //Puts back the separator and moves the str pointer
    s[separatorPos] = separator;
    *str = s+separatorPos+1;

    return res;
}

// Reads incomming data from the esp and writes it in buffer
// If an error occurs (like a timeout), the function returns 0, otherwise 1
int readDataFromEsp(char* buffer) {
    
    int len = 0;
    long startTime = millis();
    
    // Reads the stream until a \n is found (includes the \n)
    int next = SERIALOBJECT.read();
    while(next != '\n') {

        // If the length is more than 8 then there is a problem so we empty the buffer and abort
        if(len >= 8) {
            while(SERIALOBJECT.available())
                SERIALOBJECT.read();
            return 0;
        }

        buffer[len] = next;
        len++;
        
        // Waits for the next character. Aborts if the timeout is reached
        while(!SERIALOBJECT.available())
            if(millis() - startTime > TIMEOUT)
                return 0;
            
        next = SERIALOBJECT.read();
    }   

    buffer[len] = '\n';

    return 1;
}

// Requests handling ---------------------------------------------------------------------

// Sends the entire map to the ESP through Serial
// The ESP has to be reading the data while it's being written because the
// Serial buffer can only hold 256 bytes on the Mega and 64 on the Uno
void sendMapToEsp() {
    
    for(int i = 0; i < MAP_SIZE*MAP_SIZE/8; i++) {
        
        // Gets the number from the map
        unsigned char uc = bm_getByte(internMap, i);

        // Waits for the ESP to read the data
        if(i%16 == 0)
            SERIALOBJECT.flush();
        
        // Transmits the number in hexadecimal form
        SERIALOBJECT.write(uc);
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

    // If something was received, reads it entirely
    char* buffer = (char*) malloc(sizeof(char) * 9); // The size is the biggest message possible (T<POSX>,<POSY>\n)
    char* bufferStart = buffer;
    int success = readDataFromEsp(buffer);
    if(!success) {
        free(buffer);
        return;
    }

    char commandType = buffer[0];
    buffer++;
    // If a new target position is received, updates it
    // Target position update commands are of this form: T<POSX>,<POSY>\n
    if(commandType == 'T') {
        int x = readInt(&buffer, ',');
        int y = readInt(&buffer, '\n');
        vectorSet(target, x, y);
        needsPathUpdate = 1;
    }
    // If the map is requested, sends it
    // Map requests are of this form: M\n
    if(commandType == 'M')
        sendMapToEsp();
    // If the position is requested, sends it
    // Position requests are of this form: P\n
    if(commandType == 'P')
        sendPositionToEsp();  

    free(bufferStart);
}
