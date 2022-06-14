/* 
 * This is the ESP Web Tools library,For Upload Firmware using web Browser and Web Socket Terminal on Browser
 * File:   ESP_Web_Basic.ino
 * Author: Nilesh Mundphan
 * Created on June 14, 2022, 11:03 PM
 */
 
#include "ESP_Web_Tool.h"

const char* hssid      = "JioFiber-Home-2G";
const char* hpassword  = "iot19internet9229";

const char* ssid1       = "ESP32Server";
const char* password1   = "n1234567890";

#define LED 2

long lastmillis=0;
long blink_delay=1000;

//ESP_Webtool tool(80,1337);

ESP_Webtool tool;
int counter = 0;

void tool_callback(uint8_t* payload,unsigned int len){
  Serial.println("In Main Code");
  Serial.println((char*)payload);
}

void setup(){
    Serial.begin(115200); // Used for messages and the C array generator
    delay(100);
    Serial.println("WebTool test!");
    WiFi.begin(hssid, hpassword);
    int i =0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if(i++>20)
        {
          i=0;
          WiFi.reconnect();
          Serial.println("reconnect");        
        }
    }
    Serial.println(); 
    Serial.println("WiFi connected"); 
    Serial.print("IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println();

    WiFi.softAP(ssid1, password1);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    Serial.println(); 
    Serial.println("Setup"); 
    tool.setup();
    tool.setCallback(tool_callback);
    pinMode(LED, OUTPUT);    
    Serial.println("Loop");
    if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
   tool.listFiles();
}

void loop(){
    tool.loop();
    if(millis()-lastmillis > blink_delay){
        digitalWrite(LED, !digitalRead(LED));
        logs("Board LED " +String(digitalRead(LED)?"OFF":"ON ") + " Request: " +String(counter++));
        lastmillis = millis();
    }
}

void logs(String log_str){
  Serial.println(log_str);
  tool.print(log_str);
}