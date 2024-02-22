#include <WiFi.h>
#include <SPI.h>
#include <SD.h>
//library for Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CSPIN 10 

//for oled display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Define the pins for I2C communication
#define OLED_SDA 8
#define OLED_SCL 9

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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


  if (!SD.begin(CSPIN)) {
    Serial.println("SD Card Mount Failed");
    return;
  }

  File file = SD.open("/temperature_log.txt", FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  file.println("Temperature (C), Humidity (%)");
  file.close();
  Serial.println("SD Card initialized.");


  //for oled display 
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Check the I2C address of your display, common values are 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0);     // Start at top-left corner
  display.println(F("Hello, ESP32!"));
  
  // Display incoming server data here
  // Example: display.println(serverData);
  
  display.display();



}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {  // If a new client connects,
    Serial.println("New Client Connected");
    File file = SD.open("/temperature_log.txt", FILE_APPEND);
    while (client.connected()) {  // While the client is connected,
      if (client.available()) {  // If there's data available from the client,
        String message = client.readStringUntil('\n');  // Read the data until a newline is received
        Serial.print("Received: ");
        Serial.println(message);  // Print the received message

        //showing data with oled display 
        display.clearDisplay();
        display.setCursor(0,0);
        display.println(message);
        display.display();


        //writing to sd card 
        if (file) {
          
          file.println(message);
          file.close();
          Serial.println("Data written to SD Card.");
        } else {
        Serial.println("Error opening file for appending");
        }


        client.println("This is From server");  // Send acknowledgment back to the client
      }
    }

    client.stop();  // Disconnect the client
    Serial.println("Client Disconnected");
  }
}
