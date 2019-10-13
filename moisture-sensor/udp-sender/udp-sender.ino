#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000

// Time ESP32 will go to sleep (in seconds)
#define TIME_TO_SLEEP 10

// holds the total boot counts
RTC_DATA_ATTR int bootCount = 0;

// WiFi network name and password:
const char * networkName = "network";
const char * networkPswd = "-(igorpass)-";

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char * udpAddress = "htpc.coontie.com";
const int udpPort = 3333;

//The udp library class
WiFiUDP udp;

// Moisture sensor is connected to GPIO 34 (Analog ADC1_CH6)
const int sensorPin = 34;

// this function reads the sensor values
int getSensorValue()
{
  // how many readings to take
  #define MAX_SENSOR_READS 10

  // variable for storing the moisture sensor value
  int sensorValue = 0;

  // Reading moisture sensor values
  for (int i = 0; i < MAX_SENSOR_READS; i++)
  {
    sensorValue += analogRead(sensorPin);
    delay(20);
  }
  sensorValue = sensorValue / MAX_SENSOR_READS;

  return sensorValue;
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Connect to the WiFi network
  WiFi.begin(networkName, networkPswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }

  // schedule a wakeup in the future
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Reserve memory space for your JSON data
  StaticJsonDocument<200> doc;

  //serialize the JSON document
  doc["mac"] = String(WiFi.macAddress());
  doc["feedName"] = "moisture";
  doc["value"] = getSensorValue();
  doc["bootCount"] = bootCount;

  //This initializes the transfer buffer
  udp.begin(WiFi.localIP(), udpPort);

  //Send a packet
  udp.beginPacket(udpAddress, udpPort);
  serializeJson(doc, Serial);
  serializeJson(doc, udp);
  udp.println();
  udp.endPacket();

  esp_deep_sleep_start();
}

void loop()
{
}
