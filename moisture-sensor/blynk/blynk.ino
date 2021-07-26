/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_DEBUG        // Optional, this enables more detailed prints

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
char auth[] = "mw3uSlvpZIfaWStG_p9S0haap0_zKvyf";

// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000

// Time ESP32 will go to sleep (in seconds)
#define TIME_TO_SLEEP 900     

// holds the total boot counts
RTC_DATA_ATTR int bootCount = 0;

// Your WiFi credentials.
char ssid[] = "network";
char pass[] = "-(igorpass)-";

// Moisture sensor is connected to GPIO 34 (Analog ADC1_CH6) 
const int sensorPin = 34;

// this function reads the sensor values and send them to blynk
void publishSensorValues()
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
  
  // Sending to blynk
  Blynk.virtualWrite(V0, sensorValue); 
 
  // Send uptime
  Blynk.virtualWrite(V1, bootCount);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  //delay(500); //Take some time to open up the Serial Monitor

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  // schedule a wakeup in the future
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // setup the connection to blynk
  Blynk.config(auth);

  // setup WiFi
  Blynk.connectWiFi(ssid, pass);

  //will continue trying to connect to Blynk server. 
  //Returns true when connected, false if timeout has been reached. 
  //Default timeout is 30 seconds.
  bool result = Blynk.connect();

  if (result)
  {
    // read and publish the sensor values
    publishSensorValues();
  }

  Serial.println("Going to sleep now");
  Serial.flush(); 

  Blynk.disconnect();
  
  esp_deep_sleep_start();
}

void loop()
{
}
