#include <WiFi.h>

const char* ssid = "ESP32-Access-Point";      // SSID of your AP
const char* password = "123456789";           // Password of your AP
const char* host = "192.168.4.1";             // IP address of the server
const uint16_t port = 23;                     // The port on which the server is listening

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);  // Connect to the WiFi network

  while (WiFi.status() != WL_CONNECTED) {  // Wait for the connection to complete
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi. Type your messages in the Serial Monitor.");
}

void loop() {
  if (Serial.available() > 0) {  // Check if the user has entered data into the Serial Monitor
    WiFiClient client;

    if (!client.connect(host, port)) {  // Attempt to connect to the TCP server
      Serial.println("Connection to server failed");
      return;
    }

    Serial.println("Connected to server, sending message...");
    while (Serial.available() > 0) {
      char c = Serial.read();  // Read a character from the Serial Monitor
      client.write(c);         // Send the character to the server
    }
    client.println();  // Ensure the message is terminated (so the server knows it's the end of the message)

    // Wait for the server to send back a response
    while (client.connected() && !client.available()) {
      delay(1);  // Wait for a response
    }

    // Read the response from the server and print it
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    Serial.println("\nServer response received.");
    client.stop();  // Disconnect from the server
  }
}
