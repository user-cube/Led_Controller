#include <ESP8266WiFi.h>

/**
 * Credentials to enter in your network.
 */
const char* ssid     = "SSID";
const char* password = "PASSWORD";

/**
 * Set web server port number to 80.
 */
WiFiServer server(80);

/**
 * Store the HTTP Request.
 */
String header;

/**
 * Store the light state
 */
String blueState = "off";
String whiteState = "off";
String yellowState = "off";
String redState = "off";
String greenState = "off";

/**
 * Assign output variable to GPIO pins
 */
const int blue = 14;
const int white = 13;
const int yellow = 12;
const int red = 5;
const int green = 4;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  /**
   * Initialize the output variables as outputs
   */
  pinMode(blue, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  
  /**
   * Set outputs to low.
   */
  digitalWrite(blue, LOW);
  digitalWrite(white, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);

  /**
   * Connect to WIFI Network.
   */
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();         
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            /**
             * Turn GPIO to HIGH or LOW.
             * 
             * LOW -> OFF
             * HIGH -> ON
             */
             if (header.indexOf("GET /14/on") >= 0) {
              blueState = "on";
              digitalWrite(blue, HIGH);
            } else if (header.indexOf("GET /14/off") >= 0) {
              blueState = "off";
              digitalWrite(blue, LOW);
            } else if (header.indexOf("GET /13/on") >= 0) {
              whiteState = "on";
              digitalWrite(white, HIGH);
            } else if (header.indexOf("GET /13/off") >= 0) {
              whiteState = "off";
              digitalWrite(white, LOW);
            }else if (header.indexOf("GET /12/on") >= 0) {
              yellowState = "on";
              digitalWrite(yellow, HIGH);
            } else if (header.indexOf("GET /12/off") >= 0) {
              yellowState = "off";
              digitalWrite(yellow, LOW);
            }else if (header.indexOf("GET /5/on") >= 0) {
              redState = "on";
              digitalWrite(red, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              redState = "off";
              digitalWrite(red, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              greenState = "on";
              digitalWrite(green, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              greenState = "off";
              digitalWrite(green, LOW);
            }
            
            /**
             * Webpage Display.
             */
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>IOT Controler</h1>");
            client.println("<table>");
            client.println("<tr><th>Led Color</th><th>State</th><th>Action</th></tr>");
            
            client.println("<tr>");
            client.println("<td>White</td><td>"+ whiteState + "</td>");     
            if (whiteState=="off") {
              client.println("<td><a href=\"/13/on\"><button class=\"button\">ON</button></a></td>");
            } else {
              client.println("<td><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></td>");
            } 
            client.println("</tr>");
            
            client.println("<tr>");
            client.println("<td>Yellow</td><td>"+ yellowState + "</td>"); 
            if (yellowState=="off") {
              client.println("<td><a href=\"/12/on\"><button class=\"button\">ON</button></a></td>");
            } else {
              client.println("<td><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></td>");
            }
            client.println("</tr>");

            client.println("<tr>");
            client.println("<td>Blue</td><td>"+ blueState + "</td>"); 
            if (blueState=="off") {
              client.println("<td><a href=\"/14/on\"><button class=\"button\">ON</button></a></td>");
            } else {
              client.println("<td><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></td>");
            } 
            client.println("</tr>");
            
            client.println("<tr>");
            client.println("<td>Green</td><td>"+ greenState + "</td>"); 
            if (greenState=="off") {
              client.println("<td><a href=\"/4/on\"><button class=\"button\">ON</button></a></td>");
            } else {
              client.println("<td><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></td>");
            } 
            client.println("</tr>");
            
            client.println("<tr>");
            client.println("<td>Vermelho</td><td>"+ redState + "</td>"); 
            if (redState=="off") {
              client.println("<td><a href=\"/5/on\"><button class=\"button\">ON</button></a></td>");
            } else {
              client.println("<td><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></td>");
            } 
            client.println("</tr>");

            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
}
