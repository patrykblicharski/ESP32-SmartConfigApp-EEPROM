/*
  Date: 24.07.2022
  Code is written by: Blacha
  SmartConfig with EEPROM for credentials storage
*/
#include "WiFi.h"

String ssid;                        //string with SSID - todo
String pss;                         //string with password to wifi host

void setup() {
  Serial.begin(115200);             //Init serial out
  
  ssid = WiFi.SSID();             //#temp - get default generated ssid
  pss = WiFi.psk();               //#temp - get default generated passwd

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

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("SSID:");
    Serial.println(ssid);
    Serial.print("PSS:");
    Serial.println(pss);
   
    
  }
  else
  {
    Serial.println("WiFi Connected");
  }
}

void loop() {
 
}