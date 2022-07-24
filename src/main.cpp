/*
  Date: 24.07.2022
  Code is written by: Blacha
  SmartConfig with EEPROM for credentials storage
*/
#include "WiFi.h"
#include "EEPROM.h"

#define EEPROM_SIZE 200             // EEPROM size
#define WiFi_rst 0                  //WiFi config reset pin
#define LENGTH(x) (strlen(x) + 1)   //func. lenght of char string
String ssid;                        //string with SSID - todo
String pss;                         //string with password to wifi host

unsigned long rst_millis;           //button interval to restart config

void writeStringToFlash(const char* toStore, int startAddr) {
  int i = 0;
  for (; i < LENGTH(toStore); i++) {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}

String readStringFromFlash(int startAddr) {
  char in[128];                       // char array of size 128 for reading the stored data 
  int i = 0;
  for (; i < 128; i++) {
    in[i] = EEPROM.read(startAddr + i);
  }
  return String(in);
}
//-----------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);             //Init serial out
  
  pinMode(WiFi_rst, INPUT);

  if (!EEPROM.begin(EEPROM_SIZE)) { //Init EEPROM
    Serial.println("!failed to initialize EEPROM");
    delay(1000);
  }
else
  {
    ssid = readStringFromFlash(0); // Read SSID stored at address 0
    Serial.print("SSID = ");
    Serial.println(ssid);
    pss = readStringFromFlash(40); // Read Password stored at address 40
    Serial.print("psss = ");
    Serial.println(pss);
  }
 
  WiFi.begin(ssid.c_str(), pss.c_str());

  delay(3500);   // Wait for a while till ESP connects to WiFi

  if (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.mode(WIFI_AP_STA);         //Init WiFi as Station
    WiFi.beginSmartConfig();        //Start SmartConfig

    Serial.println("Waiting for App packet");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Packet received.");


    Serial.println("Waiting for WiFi");     //Wait for WiFi to connect to AP
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("WiFi established.");

    ssid = WiFi.SSID();
    pss = WiFi.psk();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("SSID:");
    Serial.println(ssid);
    Serial.print("PSS:");
    Serial.println(pss);
   
    writeStringToFlash(ssid.c_str(), 0); // storing ssid 
    writeStringToFlash(pss.c_str(), 40); // storing pss 
  }
  else
  {
    Serial.println("WiFi Connected!");
  }
}

void loop() 
{
  rst_millis = millis();
  while (digitalRead(WiFi_rst) == LOW) {}
  if (millis() - rst_millis >= 3000)
    {
      Serial.println("Reseting the WiFi credentials data");
      writeStringToFlash("", 0); // Reset the SSID
      writeStringToFlash("", 40); // Reset the Password
      Serial.println("Wifi credentials erased");
      Serial.println("Restarting the ESP");
      delay(500);
      ESP.restart();  
    }

 
}