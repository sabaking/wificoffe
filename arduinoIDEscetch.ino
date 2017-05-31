#include <ESP8266WiFi.h>

const char* ssid = "login";
const char* password = "pass";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  //digitalWrite(2, 1);

  // prepare GPIO4
  pinMode(4, OUTPUT);
  //digitalWrite(4, 1);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  digitalWrite(2, 1); // clean
  digitalWrite(4, 1); // clean
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("makecoffee") != -1){
    Serial.println("ACTION MAKE");
    // Push power button
    digitalWrite(2, 0); // on
    delay(300);
    digitalWrite(2, 1); // off
    // Push button for make coffe
    digitalWrite(4, 0); // on
    delay(300);
    digitalWrite(4, 1); // off
  }
  else if (req.indexOf("stop") != -1){
    Serial.println("ALERT STOP");    
    digitalWrite(4, 0); // on
    delay(300);
    digitalWrite(4, 1); // off
    
    digitalWrite(2, 0); // on
    delay(300);
    digitalWrite(2, 1); // off    
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n OK ";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

}
