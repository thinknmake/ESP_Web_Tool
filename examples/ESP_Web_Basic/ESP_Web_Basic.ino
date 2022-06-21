/* 
 * This is the ESP Web Tools library,For Upload Firmware using web Browser and Web Socket Terminal on Browser
 * File:   ESP_Web_Basic.ino
 * Author: Nilesh Mundphan
 * Created on June 14, 2022, 11:03 PM
 */
 
#include "ESP_Web_Tool.h"

#define     LED         2
#define     UDATE_TIME  1000

const char* hssid       = "xxxxxxxxxxxx";
const char* hpassword   = "000000000000";

const char* ssid1       = "ESP32Server";
const char* password1   = "n1234567890";

long        lastupdate  = 0;
int         counter     = 0;

ESP_Webtool   tool;

void setup(){
    Serial.begin(115200); // Used for messages and the C array generator
    delay(100);
    Serial.println("ESP WebTool test!");
    Serial.println("Setup Started"); 
    setup_wifi();
    tool.enableDebug(true);
    tool.setup();
    tool.setCallback(tool_callback);
    pinMode(LED, OUTPUT);    
    Serial.println("Entering Loop");
}

void loop(){
    tool.loop();
    if(millis()- lastupdate > UDATE_TIME){
        digitalWrite(LED, !digitalRead(LED));
        logs("Board LED " +String(digitalRead(LED)?"OFF":"ON ") + " Request: " +String(counter++));
        lastupdate = millis();
    }
}

void setup_wifi(){
    int i =0;
    WiFi.begin(hssid, hpassword);
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
}

void logs(String log_str){
  Serial.println(log_str);
  tool.print(log_str);
}

void tool_callback(uint8_t* payload,unsigned int len){
  Serial.println("Tool CallBack");
  Serial.println((char*)payload);
}