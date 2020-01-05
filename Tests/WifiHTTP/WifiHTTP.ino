// This script is just here to figure out how to make http requests with wifi
// It makes a webserver and answers simple requests (witch is what the robot will do)

/*==========================================================================
 * Project: ArduinoBasics WiFi Scanner
 * Author:  Scott C
 * Date created: 16 September 2018
 * Arduino IDE version: 1.8.5
 * Operating System: Windows 10 Pro
 * 
 * Description: 
 * The ESP-13 WiFi shield will create an Access Point that you can connect to 
 * using your phone/tablet/PC. Once connected via WiFi, navigate your browser 
 * to the IP address displayed in the Serial monitor to see the list of Access Points in your area.
 * 
 * Acknowledgements:
 * Some of the code used within this sketch was inspired by or adapted from other notable sources.
 *    Webserver code: adapted from https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/server-examples.html
 *    WiFi scanner code adapted from tablatronix : https://gist.github.com/tablatronix/497f3b299e0f212fc171ac5662fa7b42
 *    HTTP/1.1 protocols: https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 *    
 * -------------------------------------------------------------------------   
 * LIBRARIES:
 * Libraries used: ESP8266WiFi.h  - installed with Boards Manager : https://github.com/esp8266/Arduino
 * More information about ESP8266WiFi.h library : 
 * https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
 * https://arduino-esp8266.readthedocs.io/en/2.4.2/libraries.html#wifi-esp8266wifi-library
 *                                       
 *                                       
 * GLOBAL VARIABLES:
 * The SSID and Password for the Access point (AP) that will be created by the ESP-13 WiFi shield
 * The WiFi server which will listen for Clients on Port 80 (eg. Web Browsers)
 ============================================================================ */
 
#include <ESP8266WiFi.h>

const char* ssid = "PinkFluffyUnicorn";
const char* password = "12345678";
WiFiServer server(80);



/* ==========================================================================
 * SETUP:
 * Begin Serial Communication using a Baud Rate of 115200
 * Configure the ESP-13 WiFi shield into Access Point Mode
 * Create an Access Point for devices to connect to.
 * Display the Access Point (AP) IP address in the Serial monitor.
 * Start the server if the Access Point has been created successfully
 ============================================================================ */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  boolean r = WiFi.softAP(ssid, password);

  if (r) {
    server.begin();
    Serial.println("");
    Serial.print("Please connect to the Access Point: ");
    Serial.println(ssid);
    Serial.printf("then open %s in a web browser\n", WiFi.softAPIP().toString().c_str());
    Serial.println("");
  } else {
    Serial.println("FAILED to create Access Point");
    while(1){}
  }
}




/* ==========================================================================
 * LOOP:
 * Start listening for Clients
 * Notify when a Client connects to the server using the Serial Monitor
 * Then write any request messages from the client to the Serial Monitor
 * Wait for the client to send through an empty line, then
 * Serve the constructed HTML page to the client
 * The delay gives the browser some time to receive the HTML page, before
 * the server dicsonnects from the client.
 * Notify when the client has been disconnected.
 ============================================================================ */
void loop() {
  WiFiClient client = server.available();
  if(client){
    Serial.println("\n A Client just connected to the server");
    while(client.connected()) {
      if(client.available()){
        String clientMessage = client.readStringUntil('\r');
        Serial.print(clientMessage);
        if(clientMessage.length() == 1 && clientMessage[0] =='\n') {
          client.println(constructHTMLpage());
          break;
        }
      }
    }
    delay(1000);
    client.stop();
    Serial.println("\n The server has disconnected the Client");
  }
}





/* ==========================================================================
 *constructHTMLpage:
 *SCAN for available WiFi networks within range of the ESP-13 WiFi Shield
 *Construct the necessary HTTP response and HTML to 
 *  - display the SCAN results within a web browser (client).
 *  - automatically refresh the HTML page every 5 seconds.
 *  
 *The HTML page will display the following information in a table:
 *  - SSID, 
 *  - Signal Strength and 
 *  - Encryption type.
 ============================================================================ */
String constructHTMLpage(){
  int numSSID = WiFi.scanNetworks();
  String HTMLpage = String("HTTP/1.1 200 OK\r\n") +
                            "Content-Type: text/html\r\n" +
                            "Connection: close\r\n" +
                            "Refresh: 5\r\n" +
                            "\r\n" +
                            "<!DOCTYPE HTML>" +
                            "<html><body>\r\n" +
                            "<h2>WIFI NETWORKS</h2>\r\n" +
                            "<table><tr><th>SSID</th><th>Signal Strength</th><th>Encryption type</th></tr>\r\n";
                            
  for(int i=0; i<numSSID; i++){
    HTMLpage = HTMLpage + String("<tr><td>");
    HTMLpage = HTMLpage + String(WiFi.SSID(i));
    HTMLpage = HTMLpage + String("</td><td>");
    HTMLpage = HTMLpage + String(WiFi.RSSI(i));
    HTMLpage = HTMLpage + String("</td><td>");
    HTMLpage = HTMLpage + String(encryptionTypeStr(WiFi.encryptionType(i)));
    HTMLpage = HTMLpage + String(" [");
    HTMLpage = HTMLpage + String(WiFi.encryptionType(i));
    HTMLpage = HTMLpage + String("]");
    HTMLpage = HTMLpage + String("</td></tr>");
  }
  HTMLpage = HTMLpage + String("</table></body></html>\r\n");
  return HTMLpage;
}






/* ==========================================================================
 *encryptionTypeStr:
 *Will accept a WiFi.encryptionType() integer and convert it into a more
 *meaningful string. 
 ============================================================================ */
String encryptionTypeStr(uint8_t authmode) {
    switch(authmode) {
        case ENC_TYPE_NONE:
            return "OPEN";
        case ENC_TYPE_WEP:
            return "WEP";
        case ENC_TYPE_TKIP:
            return "WPA_PSK";
        case ENC_TYPE_CCMP:
            return "WPA2_PSK";
        case ENC_TYPE_AUTO:
            return "WPA_WPA2_PSK";
        default:
            return "UNKOWN";
    }
}
