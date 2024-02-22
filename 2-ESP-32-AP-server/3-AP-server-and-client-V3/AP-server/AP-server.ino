#include <WiFi.h>

const char* ssid = "ESP32-Access-Point";  // Name of the access point
const char* password = "123456789";       // Password for the access point

WiFiServer server(23);  // Create a server that listens on port 23

void setup() {
  Serial.begin(115200);  // Start the Serial communication
  WiFi.softAP(ssid, password);  // Start the access point
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());  // Print the IP address of the server

  server.begin();  // Start listening for clients
}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {  // If a new client connects,
    Serial.println("New Client Connected");
    while (client.connected()) {  // While the client is connected,
      if (client.available()) {  // If there's data available from the client,
        String message = client.readStringUntil('\n');  // Read the data until a newline is received
        Serial.print("Received: ");
        Serial.println(message);  // Print the received message
        client.println("This is From server");  // Send acknowledgment back to the client
      }
    }
    client.stop();  // Disconnect the client
    Serial.println("Client Disconnected");
  }
}
