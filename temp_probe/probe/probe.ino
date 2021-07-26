/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
*********/
#define BLYNK_PRINT Serial
#define BLYNK_DEBUG        // Optional, this enables more detailed prints

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//*****************BEGIN Blynk setup********************
// You should get Auth Token in the Blynk App.
char auth[] = "mw3uSlvpZIfaWStG_p9S0haap0_zKvyf";

// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000

// Your WiFi credentials.
char ssid[] = "network";
char pass[] = "-(igorpass)-";


//*****************END Blynk setup********************


// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
}

void sendValueToBlynk(float value) {
  Blynk.virtualWrite(V0, value);
}

void loop() {

  //
  sensors.requestTemperatures();
  //float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  //Serial.print(temperatureC);
  //Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("F");

  //sendValueToBlynk(temperatureF);

  delay(3000);
}
