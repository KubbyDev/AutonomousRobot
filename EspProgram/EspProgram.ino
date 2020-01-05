#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
const char* ssid = "MappingRobot";

#define SERIALOBJECT Serial
#define RESPONSE_TIMEOUT 1000

//Checks if the argument is given and is a 1-3 digits positive integer
int isArgumentCorrect(String argument) {
    String value = server.arg(argument);
    if(!value)
        return 0;
    for(unsigned int i = 0; i < value.length(); i++)
        if(value[i] < '0' || value[i] > '9')
            return 0;
    return 1;
}

//Waits for the Arduino to answer to a request
//Returns 1 if the answer was received, 0 otherwise
int waitForArduino() {
    
    int i = 0;
    //Waits for the Arduino to start answering
    while(!SERIALOBJECT.available()) {
        i++;
        if(i > RESPONSE_TIMEOUT) {
            server.send(500, "text/plain", "The ESP recieved the request but the Arduino didn't respond");
            return 0;  
        }
        delay(1);
    }
    
    //Waits for the Arduino to finish writing
    delay(50);
    
    return 1;
}

// Returns the wanted chunk of the intern map (cells are grouped by 8 and sent as the decimal
// representation of these 8 bits. So each group is encoded as 3 characters from 0 to 9)
// Query is of this form: /map_chunk?index=000
void getMapChunk() { 

    // Stops if the argument is not given or not a number
    if(!isArgumentCorrect("index")) {
        server.send(400, "text/plain", "Bad arguments. Query is of this form: /map_chunk?index=000");
        return;
    }
    // Sends a request to the Arduino to get the map chunk
    String message = "M" + server.arg("index") + "\n";
    SERIALOBJECT.write(message.c_str(), message.length());

    // Waits for the Arduino to respond
    if(!waitForArduino())
        return;

    // Reads and sends the response of the Arduino
    String res = "";        
    char next = SERIALOBJECT.read();
    while(SERIALOBJECT.available() && next != '\n') {
        res += next;
        next = SERIALOBJECT.read();
    }
    server.send(200, "text/plain", res);
}

// Sets the new target position of the robot
// Query is of this form: /set_target?x=000&y=000
void setTarget() {
    // Stops if one of the argument is not given or not a number
    if(!isArgumentCorrect("x") || !isArgumentCorrect("y")) {
        server.send(400, "text/plain", "Bad arguments. Query is of this form: /set_target?x=000&y=000");
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
    
    // Waits for the Arduino to respond
    if(!waitForArduino())
        return;

    // Reads and sends the response of the Arduino
    String res = "";        
    char next = SERIALOBJECT.read();
    while(SERIALOBJECT.available() && next != '\n') {
        res += next;
        next = SERIALOBJECT.read();
    }
    server.send(200, "text/plain", res);
}

void setup() {

    SERIALOBJECT.begin(250000);
    IPAddress local_IP(192,168,0,20);
    IPAddress gateway(192,168,0,1);
    IPAddress subnet(255,255,255,0);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP("ESPsoftAP_01");
    WiFi.softAPIP();

    WiFi.softAP(ssid);
    server.on("/map_chunk", getMapChunk);
    server.on("/set_target", setTarget);
    server.on("/position", getPosition);
    server.begin();
}

void loop() {
    server.handleClient();
}
