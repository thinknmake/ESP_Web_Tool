/* 
    Copyright (c) 2021 Think Make. All rights reserved.
    Author : Nilesh Mundphan
    Date  : 22:04:21  20:00 IST 
*/

#ifndef _ESP_WEB_TOOL_
#define _ESP_WEB_TOOL_

#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#elif ESP32
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Update.h>
#endif

#include <Arduino.h>

class ESP_Webtool
{
        public: 
            ESP_Webtool();
            ESP_Webtool(uint16_t port,uint16_t port1);  
            
            void print(String logs);
            void sock_callback();   
            void setup();
            void loop(); 
            
            void root_page(void);
            void uploadResp(void);
            void handleUpload(void);
            void onWebSocketEvent(uint8_t client_num, WStype_t type,uint8_t * payload,size_t length);

    protected:
    #ifdef ESP8266
            ESP8266WebServer server;
    #elif ESP32
            WebServer server;
    #endif
    
            WebSocketsServer *webSocket;
            boolean fs_enable=false;
            boolean restart  = false;
            boolean websockisConnected = true; 
};

#endif