#include <Arduino.h>
#include "ESP_Web_Tool.h"
#include "WebHTML.h"

ESP_Webtool::ESP_Webtool():server(80){
  webSocket = new WebSocketsServer(1337);
  server.on("/", HTTP_GET,std::bind(&ESP_Webtool::root_page,this));
  server.on("/update", HTTP_POST, std::bind(&ESP_Webtool::uploadResp,this),std::bind(&ESP_Webtool::handleUpload,this));
}

ESP_Webtool::ESP_Webtool(uint16_t port,uint16_t port1):server(port){    
  webSocket = new WebSocketsServer(port1);
  server.on("/", HTTP_GET,std::bind(&ESP_Webtool::root_page,this));
  server.on("/update", HTTP_POST, std::bind(&ESP_Webtool::uploadResp,this),std::bind(&ESP_Webtool::handleUpload,this));
} 

void ESP_Webtool::setup(){
  using namespace std::placeholders;
  server.begin();   
  webSocket->begin();
  webSocket->onEvent(std::bind(&ESP_Webtool::onWebSocketEvent,this,_1,_2,_3,_4));
} 

void ESP_Webtool::loop(){
  server.handleClient();
  webSocket->loop(); 
} 

void ESP_Webtool::print(String logs){
  if(websockisConnected){
    webSocket->sendTXT(0,logs);
  }
}

void ESP_Webtool::onWebSocketEvent(uint8_t client_num, WStype_t type,uint8_t * payload,size_t length) 
{
  // Figure out the type of WebSocket event
  switch(type) {
    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      websockisConnected=false;
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket->remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
        websockisConnected=true;
      }
      break;

    // Handle text messages from client
    case WStype_TEXT:
      // Print out raw message
      Serial.printf("[%u] Received text: %s\n", client_num, payload);
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

void ESP_Webtool::uploadResp(void) {  
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  delay(1000);
  if(restart){
    ESP.restart();
  }  
}

void ESP_Webtool::root_page(void) {  
  server.sendHeader("Connection", "close");
  server.send(200, "text/html", index_html);
}

void ESP_Webtool::handleUpload(void){  
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      String filename = upload.filename;
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if(filename.endsWith(".bin")){
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        print("Updating Firmware ...");
        if (!Update.begin(maxSketchSpace)) { //start with max available size
          Update.printError(Serial);
        }
      }else{
          Serial.println("Not Bin file");
          print("Not Bin file\n");
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      String filename = upload.filename;
      if(filename.endsWith(".bin")){
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      }else{
          //Serial.println("Not Bin file\n");
          //print("Not Bin file\n");
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      String filename = upload.filename;
        if(filename.endsWith(".bin")){
          if (Update.end(true)) { //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            print("Rebooting...\n");
            restart = true;
          } else {
            Update.printError(Serial);
          }
        }
        else{
          Serial.println("Not Bin file");
          print("Not Bin file\n");
          restart = false;
        }
   }
}

void ESP_Webtool::sock_callback(){

}
