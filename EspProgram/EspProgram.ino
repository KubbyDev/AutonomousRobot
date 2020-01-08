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

ESP8266WebServer server(80);
const char* ssid = "MappingRobot"; //Name of the Wifi network. Connect to it to send HTTP requests

#define SERIALOBJECT Serial //The serial object used to communicate with the Arduino

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

//Reads data from the Arduino while it is writing it
String receiveData() {

    String res = "";

    //Waits for the first character
    while(!SERIALOBJECT.available());
    
    //Reads the stream until a \n is found
    char next = SERIALOBJECT.read();
    while(next != '\n') {
        
        res += next;
        
        //Waits for the next character
        while(!SERIALOBJECT.available());
        next = SERIALOBJECT.read();
    }

    return res;
}

// Returns the intern map (cells are grouped by 8 and sent as the hexadecimal
// representation of these 8 bits. So each group is encoded as 2 characters from 0 to F)
// Query is of this form: /map
void getMap() { 

    // Sends a request to the Arduino to get the map 
    SERIALOBJECT.write("M\n", 2);

    // Gets the response from the Arduino
    String res = receiveData();

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

    // Gets the response from the Arduino
    String res = receiveData();
    
    server.send(200, "text/plain", res);
}

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
    server.begin();
}

void loop() {
    server.handleClient();
}
