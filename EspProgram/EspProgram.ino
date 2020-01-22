// -------------------------------------------------------------------------------------------------
//
// This code is meant to control the ESP8266. Functionnalities:
// - Hosts the Wifi network (ssid: MappingRobot, open)
// - Receives HTTP requests and checks if they are correct
// - Communicates with the Arduino via Serial to answer or serve requests
//
// Requests:
// - /map: Returns the intern map stored on the Arduino in the form 
// of hexadecimal digits (one digit for 4 pixels)
// - /set_target?x=000&y=000: Changes the goal location of the robot. 
// Numbers are in intern map coordinates
// - /position: Returns the expected position and rotation of the robot
// in its intern map.
//
// -------------------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SERIALOBJECT Serial // The serial object used to communicate with the Arduino
#define MAP_SIZE 72 // Size of the main map (should be the same here, in ArduinoProgram/Data.h and in the smarphone app)
#define TIMEOUT 1000 // Maximum waiting time for incomming data from the Arduino in milliseconds

// Errors that can be encoutered while communicating with the Arduino
#define SERIAL_NO_RESPONSE 1
#define SERIAL_RESPONSE_NOT_FINISHED 2
#define SERIAL_RESPONSE_TOO_LONG 3

// Ids of the requests. Used by handleError
#define ID_MAP 1
#define ID_SET_TARGET 2
#define ID_POSITION 3

ESP8266WebServer server(80);
const char* ssid = "AutonomousRobot"; // Name of the Wifi network. Connect to it to send HTTP requests

// Helper functions --------------------------------------------------------------------------------

// Converts an number from 0 to 15 to its hexadecimal representation
char toHex(int i) {
    return i < 10 ? i+'0' : i-10+'A';
}

// Checks if the argument is given and is a 1-3 digits positive integer
int isArgumentCorrect(String argument) {
    String value = server.arg(argument);
    if(!value)
        return 0;
    for(unsigned int i = 0; i < value.length(); i++)
        if(value[i] < '0' || value[i] > '9')
            return 0;
    return 1;
}

// Reads data from the Arduino while it is writing it
// The buffer in parameter should be big enough to contain the response without \n
// For example if the response is expected to be TEST\n it should be of length 4.
// The return value is an error code (see the to of this file for more info)
int receiveData(int* buffer, int bufferLength) {

    long startTime = millis();
    
    // Waits for the first character
    while(!SERIALOBJECT.available())
        if(millis() - startTime > TIMEOUT)
            return SERIAL_NO_RESPONSE;
    
    // Reads the stream until a \n is found
    int next = SERIALOBJECT.read();
    while(next != '\n') {
        
        // Prevents buffer overflows
        if(bufferLength <= 0)
            return SERIAL_RESPONSE_TOO_LONG;

        *buffer = next;
        buffer++;
        bufferLength--;
        
        // Waits for the next character
        while(!SERIALOBJECT.available())
            if(millis() - startTime > TIMEOUT)
                return SERIAL_RESPONSE_NOT_FINISHED;
            
        next = SERIALOBJECT.read();
    }

    return 0;
}

// Handles an error that occured while communicating with the Arduino
void handleError(int errorCode, int requestID) {

    int code;
    String message = "Error while processing ";
    if(requestID == ID_MAP) message += "/map request: ";
    if(requestID == ID_SET_TARGET) message += "/set_target request: ";
    if(requestID == ID_POSITION) message += "/position request: ";

    // Serial communication errors
    if(errorCode == SERIAL_NO_RESPONSE) {
        code = 500;
        message += "The Arduino didn't respond. ";
    }
    if(errorCode == SERIAL_RESPONSE_NOT_FINISHED) {
        code = 500;
        message += "The Arduino responded but an error occured while receiving it. ";
    }
    if(errorCode == SERIAL_RESPONSE_TOO_LONG) {
        code = 500;
        message += "The buffer used to collect the response was too small. ";
    }
    
    server.send(code, "text/plain", message);  
}

// Request handlers --------------------------------------------------------------------------------

// Returns the intern map (cells are grouped by 8 and sent as the hexadecimal
// representation of these 8 bits. So each group is encoded as 2 characters from 0 to F)
// Query is of this form: /map
void getMap() { 

    // Sends a request to the Arduino to get the map 
    SERIALOBJECT.write("M\n", 2);

    // Gets the response from the Arduino
    int* res = (int*) malloc(sizeof(int) * MAP_SIZE*MAP_SIZE/8);
    int error = receiveData(res, MAP_SIZE*MAP_SIZE/8);

    // Sends an error message if there is an error
    if(error != 0) {
        handleError(error, ID_MAP);
        free(res);
        return;
    }

    // Formats the received data to sends it back in the HTTP response
    String result = "";
    for(int i = 0; i < MAP_SIZE*MAP_SIZE/8; i++) {
        result += res[i]/16;
        result += res[i]%16;
    }

    server.send(200, "text/plain", result);
    free(res);
}

// Sets the new target position of the robot
// Query is of this form: /set_target?x=000&y=000
void setTarget() {
    
    // Stops if one of the argument is not given or not a number
    if(!isArgumentCorrect("x") || !isArgumentCorrect("y")) {
        server.send(422, "text/plain", "Bad arguments. Query is of this form: /set_target?x=000&y=000");
        return;
    }
    // Sends the numbers to the Arduino
    String message = "T" + server.arg("x") + "," + server.arg("y") + "\n";
    SERIALOBJECT.write(message.c_str(), message.length());
    server.send(200, "text/plain", "Target changed");
}

// Returns the position and rotation of the robot
// Query is of this form: /position
void getPosition() {
    
    // Sends the command to the Arduino
    SERIALOBJECT.write("P\n", 2);

     // Gets the response from the Arduino
    int* res = (int*) malloc(sizeof(int) * 3*15 +2);
    int error = receiveData(res, 3*15 +2);

    // Sends an error message if there is an error
    if(error != 0) {
        handleError(error, ID_POSITION);
        free(res);
        return;
    }

    String result = "";
    for(int i = 0; i < 3*15 +2; i++)
        result += (char) res[i];

    server.send(200, "text/plain", result);
    free(res);
}

void notFound() {
    server.send(404, "text/plain", server.uri() + ": Not found");
}

// Main functions ----------------------------------------------------------------------------------

void setup() {

    SERIALOBJECT.begin(250000);
    IPAddress local_IP(192,168,0,20); //Connect to this IP to send HTTP requests
    IPAddress gateway(192,168,0,1);
    IPAddress subnet(255,255,255,0);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP("ESPsoftAP_01");
    WiFi.softAPIP();

    WiFi.softAP(ssid);
    server.on("/map", getMap);
    server.on("/set_target", setTarget);
    server.on("/position", getPosition);
    server.onNotFound(notFound);
    server.begin();
}

void loop() {
    server.handleClient();
}
