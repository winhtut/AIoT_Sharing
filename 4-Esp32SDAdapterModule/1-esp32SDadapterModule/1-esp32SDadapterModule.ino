#include <SPI.h>
#include <SD.h>
#include <DHT.h>

#define DHTPIN 4     // DHT11 data pin
#define DHTTYPE DHT11   // DHT 11
#define CSPIN 10     // CS pin for SD card module

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

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
}

void loop() {
  delay(2000);  // Wait a few seconds between measurements

  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Append the data to the file
  File file = SD.open("/temperature_log.txt", FILE_APPEND);
  if (file) {
    file.print(temperature);
    file.print(", ");
    file.println(humidity);
    file.close();
    Serial.println("Data written to SD Card.");
  } else {
    Serial.println("Error opening file for appending");
  }
}
