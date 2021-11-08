#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include <ArduinoJson.h>
#include <Arduino_JSON.h>

#include <PubSubClient.h>
#include "EspMQTTClient.h"
//for LED status
#include <Ticker.h>
Ticker ticker;

int Led_OnBoard = 2;              //D4 GPIO 2
const int Dor = D7;               // D0 GPIO 16
const int ProBali1 = D1;          // D1 GPIO 5
const int ProBali2 = D2;          // D2 GPIO 4
const int ProBali3 = D0;          // D7 GPIO 13
const unsigned int TRIG_PIN = D6; // D6 GPI0 12
const unsigned int ECHO_PIN = D5; // D5 GPIO 14
// const int pompa[] = {D1 D2 D3};
EspMQTTClient client(
    "ws.spairum.my.id", // MQTT Broker server ip
    1883,               // The MQTT port, default to 1883. this line can be omitted
    "spairum",          // Can be omitted if not needed MQTTUsername
    "broker",           // Can be omitted if not needed MQTTPassword
    "ProBali"           // Client name that uniquely identify your device
);
void tick()
{
  //toggle state
  int state = digitalRead(Led_OnBoard); // get the current state of GPIO1 pin
  digitalWrite(Led_OnBoard, !state);    // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
  digitalWrite(Dor, LOW); // ON
  Serial.println("pintu terbuka");
}

void setup()
{
  Serial.begin(115200);
  pinMode(Led_OnBoard, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(Dor, OUTPUT);
  pinMode(ProBali1, OUTPUT);
  digitalWrite(ProBali1, HIGH); // OFF
  pinMode(ProBali2, OUTPUT);
  digitalWrite(ProBali2, HIGH); // OFF
  pinMode(ProBali3, OUTPUT);
  digitalWrite(ProBali3, HIGH); // OFF
  // pinMode(pompa[], OUTPUT);
  // digitalWrite(pompa[], HIGH); // OFF
  digitalWrite(Dor, LOW); // OFF
  // digitalWrite(Dor, LOW);    // ON
  // digitalWrite(Led_OnBoard, LOW);
  ticker.attach(0.6, tick);
  WiFiManager wifiManager;
  // wifiManager.autoConnect("Spairum.NET | ProBali");
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(60);

  if (!wifiManager.autoConnect("Spairum.NET | ProBali"))
  {
    Serial.println("failed Configuto connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  Serial.println("Connecting...");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  ticker.detach();
  WiFi.hostname("RKAL-ProBali-ESP8266");
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  // digitalWrite(Led_OnBoard, HIGH);
  //keep LED on
  digitalWrite(Dor, HIGH); // OFF
  // digitalWrite(Led_OnBoard, LOW);

  client.enableDebuggingMessages();                   // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater();                      // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("Offline", "ProBali"); // You can activate the retain flag by setting the third parameter to true
}

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("start/ProBali1", [](const String &payload)
                   {
                     Serial.println(payload);
                     
                     StaticJsonDocument<300> doc;
                     DeserializationError error = deserializeJson(doc, payload);
                     if (error)
                     {
                       Serial.print(F("deserializeJson() failed: "));
                       //  Serial.println(error.f_str());
                       return;
                     }
                     const char *id = doc["id"];                 // "ProBali2"
                     const char *akun = doc["akun"];             // "51E5EF69"
                     int vaule = doc["vaule"];                   // "220"
                     int faktor = doc["faktor"];                 // "1"
                     const char *HargaTotal = doc["HargaTotal"]; // "4400"
                     Serial.print(id);
                     Serial.print(akun);
                     Serial.println(vaule);
                     Serial.println("Mulai Mengisi");
                     client.publish("nodeTrans", (payload));
                      ProBali1A(vaule, akun, faktor);
                   });
  client.subscribe("start/ProBali2", [](const String &payload)
                   {
                     Serial.println(payload);
                     
                     StaticJsonDocument<300> doc;
                     DeserializationError error = deserializeJson(doc, payload);
                     if (error)
                     {
                       Serial.print(F("deserializeJson() failed: "));
                       //  Serial.println(error.f_str());
                       return;
                     }
                     const char *id = doc["id"];                 // "ProBali2"
                     const char *akun = doc["akun"];             // "51E5EF69"
                     int vaule = doc["vaule"];                   // "220"
                     int faktor = doc["faktor"];                 // "1"
                     const char *HargaTotal = doc["HargaTotal"]; // "4400"
                     Serial.print(id);
                     Serial.print(akun);
                     Serial.println(vaule);
                     Serial.println("Mulai Mengisi");
                     client.publish("nodeTrans", (payload));
                     ProBali2B(vaule, akun, faktor);
                   });
  client.subscribe("start/ProBali3", [](const String &payload)
                   {
                     Serial.println(payload);
                     
                     StaticJsonDocument<300> doc;
                     DeserializationError error = deserializeJson(doc, payload);
                     if (error)
                     {
                       Serial.print(F("deserializeJson() failed: "));
                       //  Serial.println(error.f_str());
                       return;
                     }
                     const char *id = doc["id"];                 // "ProBali2"
                     const char *akun = doc["akun"];             // "51E5EF69"
                     int vaule = doc["vaule"];                   // "220"
                     int faktor = doc["faktor"];                 // "1"
                     const char *HargaTotal = doc["HargaTotal"]; // "4400"
                     Serial.print(id);
                     Serial.print(akun);
                     Serial.println(vaule);
                     Serial.println("Mulai Mengisi");
                     client.publish("nodeTrans", (payload));
                     ProBali3C(vaule, akun, faktor);
                   });
  client.subscribe("opendor/ProBali", [](const String &payload)
                   {
                     Serial.println(payload);
                     digitalWrite(Dor, LOW); // ON
                     delay(6000);
                     digitalWrite(Dor, HIGH); // OFF
                     long rssi = WiFi.RSSI();
                     Serial.print("RSSI:");
                     Serial.println(rssi);
                     JSONVar doc;
                     doc["Sinyal"] = "ProBali";
                     doc["RSSI"] = rssi;
                     String jsonString = JSON.stringify(doc);
                     client.publish("Sinyal", jsonString.c_str());
                     JSONVar statusUP;
                     statusUP["id_mesin"] = "ProBali";
                     statusUP["Status"] = "Open Dor";
                     String jsonStringUp = JSON.stringify(statusUP);
                     client.publish("data/log", jsonStringUp.c_str());
                   });
  client.subscribe("RSSI/ProBali", [](const String &payload)
                   {
                     long rssi = WiFi.RSSI();
                     Serial.print("RSSI:");
                     Serial.println(rssi);
                     JSONVar doc;
                     doc["Sinyal"] = "ProBali";
                     doc["RSSI"] = rssi;
                     String jsonString = JSON.stringify(doc);
                     client.publish("Sinyal", jsonString.c_str());
                     JSONVar statusUP;
                     statusUP["id_mesin"] = "ProBali";
                     statusUP["Status"] = rssi;
                     String jsonStringUp = JSON.stringify(statusUP);
                     client.publish("data/log", jsonStringUp.c_str());
                     mersure();
                   });
  JSONVar statusUP;
  statusUP["id_mesin"] = "ProBali";
  statusUP["Status"] = "Restart";
  String jsonStringUp = JSON.stringify(statusUP);
  client.publish("data/log", jsonStringUp.c_str());
  mersure();
  ticker.attach(2, tick);
}

void loop()
{
  // put your main code here, to run repeatedly:
  client.loop();
}

void ProBali1A(int vaule, const char *akun, int faktor)
{
  int var = 0;
  int nilai = vaule * 100 / faktor;
  Serial.print("Mulai ");
  Serial.println(vaule);
  Serial.println(akun);
  JSONVar start;
  start["vaule"] = vaule;
  start["Status"] = ("Megisi");
  start["user"] = akun;
  String datastart = JSON.stringify(start);
  JSONVar update;
  update["vaule"] = vaule;
  update["Status"] = ("Megisi");
  update["user"] = akun;
  String dataupdate = JSON.stringify(update);
  client.publish("update", dataupdate.c_str());
  delay(2500);
  int period = 1000;
  unsigned long time_now = 0;
  time_now = millis();

  while (millis() < time_now + nilai)
  {
    //wait approx. [period] ms
    digitalWrite(ProBali1, LOW); // ON
    Serial.println("air");
    JSONVar doc;
    doc["vaule"] = vaule;
    doc["Status"] = (millis() - time_now) / 1000;
    doc["user"] = akun;
    String jsonString = JSON.stringify(doc);
    client.publish("current", jsonString.c_str());
    delay(500);
  }
  Serial.println("stop");
  digitalWrite(ProBali1, HIGH); // OFF
  JSONVar stop;
  stop["vaule"] = vaule;
  stop["Status"] = ("Selesai");
  stop["user"] = akun;
  String datastop = JSON.stringify(stop);
  client.publish("current", datastop.c_str());
}
void ProBali2B(int vaule, const char *akun, int faktor)
{
  int var = 0;
  int nilai = vaule *100 / faktor;
  Serial.print("Mulai ");
  Serial.println(vaule);
  Serial.println(akun);
  JSONVar start;
  start["vaule"] = vaule;
  start["Status"] = ("Megisi");
  start["user"] = akun;
  String datastart = JSON.stringify(start);
  JSONVar update;
  update["vaule"] = vaule;
  update["Status"] = ("Megisi");
  update["user"] = akun;
  String dataupdate = JSON.stringify(update);
  client.publish("update", dataupdate.c_str());
  delay(2500);
  int period = 1000;
  unsigned long time_now = 0;
  time_now = millis();

  while (millis() < time_now + nilai)
  {
    //wait approx. [period] ms
    digitalWrite(ProBali2, LOW); // ON
    Serial.println("air");
    JSONVar doc;
    doc["vaule"] = vaule;
    doc["Status"] = (millis() - time_now) / 1000;
    doc["user"] = akun;
    String jsonString = JSON.stringify(doc);
    client.publish("current", jsonString.c_str());
    delay(500);
  }
  Serial.println("stop");
  digitalWrite(ProBali2, HIGH); // OFF
  JSONVar stop;
  stop["vaule"] = vaule;
  stop["Status"] = ("Selesai");
  stop["user"] = akun;
  String datastop = JSON.stringify(stop);
  client.publish("current", datastop.c_str());
}
void ProBali3C(int vaule, const char *akun, int faktor)
{
  int var = 0;
  int nilai = vaule * 100 / faktor;
  Serial.print("Mulai ");
  Serial.println(vaule);
  Serial.println(akun);
  JSONVar start;
  start["vaule"] = vaule;
  start["Status"] = ("Megisi");
  start["user"] = akun;
  String datastart = JSON.stringify(start);
  JSONVar update;
  update["vaule"] = vaule;
  update["Status"] = ("Megisi");
  update["user"] = akun;
  String dataupdate = JSON.stringify(update);
  client.publish("update", dataupdate.c_str());
  delay(2500);
  int period = 1000;
  unsigned long time_now = 0;
  time_now = millis();

  while (millis() < time_now + nilai)
  {
    //wait approx. [period] ms
    digitalWrite(ProBali3, LOW); // ON
    Serial.println("air");
    JSONVar doc;
    doc["vaule"] = vaule;
    doc["Status"] = (millis() - time_now) / 1000;
    doc["user"] = akun;
    String jsonString = JSON.stringify(doc);
    client.publish("current", jsonString.c_str());
    delay(500);
  }
  Serial.println("stop");
  digitalWrite(ProBali3, HIGH); // OFF
  JSONVar stop;
  stop["vaule"] = vaule;
  stop["Status"] = ("Selesai");
  stop["user"] = akun;
  String datastop = JSON.stringify(stop);
  client.publish("current", datastop.c_str());
}
// void pause()
// {
//   Serial.println("pause");
// }

void mersure()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration / 29 / 2;
  JSONVar merter;
  merter["vaule"] = distance;
  merter["id_mesin"] = "ProBali";
  String datamerter = JSON.stringify(merter);
  client.publish("mersure", datamerter.c_str());
}
