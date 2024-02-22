#include <WiFi.h>
#include <DHT.h>

// DHT Sensor Setup
#define DHTPIN 5     // What digital pin the DHT11 is connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Server details
const char* host = "192.168.4.1";
const uint16_t port = 23;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  delay(2000);
  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print(temperature);

  // Check if any reads failed and exit early (to try again).
  // if (isnan(humidity) || isnan(temperature)) {
  //   Serial.println("Failed to read from DHT sensor!");
  //   return;
  // }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Connect to the server
  Serial.println("Connecting to server...");
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Connection to server failed");
    return;
  }

  // Send temperature and humidity to the server
  client.print("Temperature: ");
  client.print(temperature);
  client.print(" °C, Humidity: ");
  client.print(humidity);
  client.println(" %");

  // Close the connection
  client.stop();

  // Wait a second before sending the data again
  delay(1000);
}
