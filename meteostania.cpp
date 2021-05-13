#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include "ESP8266WiFi.h"
#include "ESP8266mDNS.h"
#include "WiFiUdp.h"
#include "ArduinoOTA.h"
#include <WiFiClientSecure.h>
LiquidCrystal_I2C lcd(0x27,16,2);

Adafruit_BME280 bme; 
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();
const char* ssid = "******";
const char* password = "******";
void setup() {
  Serial.begin(115200);
Wire.begin();
    if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1) delay(10);
  }
  
  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
    lcd.init();      
  lcd.backlight();
    Serial.println("Booting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }
ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
}

void loop() {
  
    ArduinoOTA.handle();
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Humidity = "));
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.println();
  
lcd.init();                     
lcd.backlight();
lcd.print("temp:      hum:");
lcd.setCursor(1, 1);
lcd.print(temp_event.temperature);
lcd.setCursor(8, 1);
lcd.print(humidity_event.relative_humidity);
delay(1000);
}
void skks()
  {
    WiFiClientSecure client;
client.setInsecure();
    if (!client.connect(server,443)) {
 Serial.println("connection failed");
 //return; 
} 

String postStr = "/update?api_key=" + apiKey + "&field1=" + int(pamiat_pm) + "&field2=" + int(T1) + "&field3=" + int(T2)+ "\r\n\r\n";
//client.print(postStr);

//Serial.println(postStr);
client.print(String("GET ")+ postStr + " HTTP/1.1\r\nHost: " + server + "\r\nConnection: close\r\n\r\n");

  String line = client.readStringUntil('\n');
  Serial.println("Feedback from Thingspeak.com:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
client.stop();
  }