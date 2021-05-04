/* 
 * This is the ESP Web Tools library,For Upload Firmware using web Browser and Web Socket Terminal on Browser
 * File:   ESP_Web_Basic.ino
 * Author: Nilesh Mundphan
 * Created on March 03, 2021, 11:03 PM
 */
#include "ESP_Web_Tool.h"
const char* hssid      = "xxxxxxxx";
const char* hpassword  = "00000000";

#define LED 2

long lastmillis=0;
long blink_delay=1000;

//ESP_Webtool tool(80,1337);
ESP_Webtool tool;

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
    Serial.println("Setup"); 
    tool.setup();
    pinMode(LED, OUTPUT);    
    Serial.println("Loop");
}

void loop(){
    tool.loop();
    if(millis()-lastmillis > blink_delay){
        digitalWrite(LED, !digitalRead(LED));
        tool.print("LED " +String(digitalRead(LED)?"OFF":"ON"));
        lastmillis = millis();
    }
}