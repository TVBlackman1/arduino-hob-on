#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#define si7021Addr 0x40
#define absTimer D3
#define otnTimer D4
#define sda D1
#define scl D2
float humidity = 0;
float celsTemp = 0;
int sensor_pin = A0; 
int output_value ;
String apiKey = "**********"; //токен thingspeak
const char* server = "api.thingspeak.com";
const char* ssid1 = "******";
const char* password = "*******";
void setup()
{
Serial.begin(9600);
Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid1, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.onStart([]() {
    Serial.println("Start");  //  "Начало OTA-апдейта"

  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");  //  "Завершение OTA-апдейта"
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    //  "Ошибка при аутентификации"
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    //  "Ошибка при начале OTA-апдейта"
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    //  "Ошибка при подключении"
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    //  "Ошибка при получении данных"
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    //  "Ошибка при завершении OTA-апдейта"
  });
  ArduinoOTA.begin();
  Serial.println("Ready");  //  "Готово"
  Serial.print("IP address: ");  //  "IP-адрес+: "
  Serial.println(WiFi.localIP());
  WiFi.begin(ssid1, password);
pinMode(D3, OUTPUT);
digitalWrite(D3, HIGH);
pinMode(D4, OUTPUT);
digitalWrite(D4, HIGH);
Wire.begin(sda, scl);   // Инициализируем шину I2C
}

void loop()
{
  ArduinoOTA.handle();
}
void Datchik()
{                                                                                   
  unsigned int data[2];
  Wire.beginTransmission(si7021Addr);
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(500);
  Wire.requestFrom(si7021Addr, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;
  Wire.beginTransmission(si7021Addr);
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(200);
  Wire.requestFrom(si7021Addr, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
output_value= analogRead(sensor_pin);
  output_value = map(output_value,564,15,0,100);
  float temp  = ((data[0] * 256.0) + data[1]);
  celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
delay(2000);

}
  void skks()
  {
    WiFiClientSecure client;
client.setInsecure();
    if (!client.connect(server,443)) {
 Serial.println("connection failed");
 //return; 
} 

String postStr = "/update?api_key=" + apiKey + "&field1=" + String(celsTemp) + "&field2=" + String(humidity) + "&field3=" + String(output_value)+ "\r\n\r\n";
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