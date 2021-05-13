
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include <WiFiClientSecure.h>

#define ONE_WIRE_BUS D1


#define PERIOD 30000
#include <BlynkSimpleEsp8266.h>
#include "Stepper_28BYJ_48.h"
const char* ssid = "******";
const char* password = "******";
#define LED_PIN D5
#define in1 D0
#define in2 D2
#define in3 D3
#define in4 D4
int PAMIAT_pm_addr;
int PAMIAT_addr;
int lul = 1;
int stepCounter = 0;
int flagDirectRotate = 1;
int setStep = 845;
int pamiat = 0;
int pamiat_dop = 0;
int pamiat_pm = 0;
uint32_t kekus = 0;
char auth[] = "5rn4LJULPpcfhH1FrClyTV3wwieF9Mqm";
uint32_t timer_3 = 0;
uint32_t timer_f1 = 0;
uint32_t timer_f2 = -10000;
uint32_t Time_prod = 0;
uint32_t Time_och = 0;
unsigned long currentMillis = 0;
float tempC = 1;
int x = 1;
String currentTime = String(0) + ":" + 0 + ":" + 0;
int currentTimeHour = 0;
int currentTimeMinutes = 0;
int currentTimeSec = 0;
int T1 =0;
int T1_pam = 1;
int T2 =0;
int y = 1;
int z = 1;
int op = 1;
int addr = 0;
int addr_pm = 0;
bool flagPwrOn = 0;
//WidgetRTC rtc;
Stepper_28BYJ_48 stepper(in1,in2,in3,in4);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1 = { 0x28, 0xAC, 0x57, 0x56, 0xB5, 0x01, 0x3C, 0x7A };
DeviceAddress sensor2 = { 0x28, 0xFD, 0xAA, 0x56, 0xB5, 0x01, 0x3C, 0x29 };

void rotateCW()
{
    
    stepper.step(-1);
    stepCounter++;
    Serial.println(stepCounter);
  
}
void rotateCCW()
{
   
    stepper.step(1);
    stepCounter--;
 Serial.println(stepCounter);
}
void sendTime() 
{  if(millis()>timer_f1+1000)
  {
  timer_f1=millis();
  }
}
void sendTemp()
{
  sensors.requestTemperatures();
T1= sensors.getTempC(sensor1);
T2= sensors.getTempC(sensor2) - 1;
  Blynk.virtualWrite(V12, T1);
  Blynk.virtualWrite(V13, T2);
} 
void setup() 
{
Serial.begin(9600);
Serial.println("Booting");  //  "Загрузка"
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    //  "Соединиться не удалось! Перезагрузка..."
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
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");  //  "Готово"
  Serial.print("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
  WiFi.begin(ssid, password);
    pinMode(LED_PIN, OUTPUT);
    //pinMode(POT_PIN, INPUT);
    timer.setInterval(30000L, sendTemp);
     EEPROM.begin(512);
}


void loop() 
{
  ArduinoOTA.handle(); 
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  currentTimeHour = hour();
  currentTimeMinutes = minute();
  currentTimeSec = second();
  if (flagPwrOn == 0)
{
   EEPROM.get(addr,pamiat);
    EEPROM.commit();
    EEPROM.get(addr_pm,pamiat_pm);
     EEPROM.commit();
 flagPwrOn = 1;
}
  Serial.println(addr);
  Serial.println(addr_pm);
    if(pamiat_pm == 1)
  {
    analogWrite(LED_PIN, 900);
    Serial.println("pamiat_pm == 1");
  }
  if(pamiat_pm == 2)
  {
    analogWrite(LED_PIN, 816);
    Serial.println("pamiat_pm == 2");
  }
  if(pamiat_pm == 3)
  {
    analogWrite(LED_PIN, 612);
   Serial.println("pamiat_pm == 3");
  }
  if(pamiat_pm == 4)
  {
   analogWrite(LED_PIN, 408);
  Serial.println("pamiat_pm == 4");
  }
  if(pamiat_pm == 5)
  {
   analogWrite(LED_PIN, 204);
   Serial.println("pamiat_pm == 5");
  }
  if(T1 > -10000000)
  {
  if(pamiat == 1)
{
  pamiat_dop = 1;
if(stepCounter == 0 || stepCounter < setStep)
{
rotateCW();
}
if(stepCounter == setStep || stepCounter > 0)
{
rotateCCW();
}
x = 1;
}

if(pamiat == 2)
{
  pamiat_dop = 2;
if(stepCounter == setStep || stepCounter > 0)
{
rotateCCW();
}
if(stepCounter == 0 || stepCounter < setStep)
{
rotateCW();
}
x = 1;
}
if(pamiat == 3)
{
  pamiat_dop = 3;
if(stepCounter == 0 || stepCounter == setStep) 
{
  if(millis()>timer_3+30000)
  {
  flagDirectRotate = !flagDirectRotate;// Дошли до нуля или предельного значения, меняем направление
  timer_3=millis();
  }
}
if(!flagDirectRotate && stepCounter < setStep)rotateCW(); //Если 0, то крутим по часовой
if(flagDirectRotate && stepCounter > 0)rotateCCW();  //Если 1, то крутим против часовой
x = 1;
}
if(pamiat == 4)
{
  if(stepCounter == 0 || stepCounter < setStep)
{
rotateCW();
}
if(stepCounter == setStep || stepCounter > 0)
{
rotateCCW();
}
x = 1;
}

  }
  else
  {
    if(stepCounter == 0 || stepCounter < setStep)
{
rotateCW();
}
if(stepCounter == setStep || stepCounter > 0)
{
rotateCCW();
}
    if(x == 1)
    {
      kekus = millis();
      x =0;
    }
if (millis() - kekus >= 240000)
{
  pamiat = 3;
  //temp
  }
}
if (currentTimeHour % 12 == 0 && currentTimeHour != 0 )
{
  pamiat = 0;
  lul = 1;
  if(stepCounter == 0 || stepCounter < setStep)
{
rotateCW();
}
x = 1;
if ( op = 1)
{
  analogWrite(LED_PIN, 204);
  op =0;
}
}
if (currentTimeHour % 12 == 0 && currentTimeHour != 0 && currentTimeMinutes == 10 && currentTimeSec == 0 && lul == 1)
{
    pamiat = pamiat_dop;
    lul = 0;
    op = 1;
    analogWrite(LED_PIN, 612);
   Serial.println("pamiat_pm == 3");
}
}
void V1()
  {
    bool PRIbuttonBlynk = param.asInt();
  
  if(PRIbuttonBlynk == HIGH)
{ 
  pamiat = 1;
eeprom_work();
}

 }
void V2()
  {
    bool VITbuttonBlynk = param.asInt();
  
  if(VITbuttonBlynk == HIGH)
{ 
  pamiat = 2;
eeprom_work();
}
 }
void V3()
  {
    bool RECbuttonBlynk = param.asInt();
  
  if(RECbuttonBlynk == HIGH)
{ 
  pamiat = 3;
eeprom_work();
}

 }
void V4()
  {
    bool OCHbuttonBlynk = param.asInt();
  
  if(OCHbuttonBlynk == HIGH)
{ 
  pamiat = 4;
eeprom_work();
}
 }
void V5()
  {
    
    bool RAZbuttonBlynk = param.asInt();
  
  if(RAZbuttonBlynk == HIGH)
{ 
  pamiat_pm = 1;
}
eeprom_work_pm();
 }
void V6()
  {
    
    bool DVAbuttonBlynk = param.asInt();
  
  if(DVAbuttonBlynk == HIGH)
{ 
  pamiat_pm = 2;
}
eeprom_work_pm();
 }
void V7()
  {
    
    bool TRIbuttonBlynk = param.asInt();
  
  if(TRIbuttonBlynk == HIGH)
{ 
  pamiat_pm = 3;
}
eeprom_work_pm();
 }
void V8()
  {
    
    bool CHETbuttonBlynk = param.asInt();
  
  if(CHETbuttonBlynk == HIGH)
{ 
  pamiat_pm = 4;
}
eeprom_work_pm();
 }
void V9()
  {
    bool PIATbuttonBlynk = param.asInt();
  
  if(PIATbuttonBlynk == HIGH)
{ 
  pamiat_pm = 5;
}
eeprom_work_pm();
 }
 void eeprom_work()
 {
  EEPROM.put(addr,pamiat);
  EEPROM.commit();
   EEPROM.end();
 }
 void eeprom_work_pm()
 {
  EEPROM.put(addr_pm,pamiat_pm);
  EEPROM.commit();
   EEPROM.end();
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