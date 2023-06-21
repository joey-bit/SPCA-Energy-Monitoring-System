#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <DS18B20.h>

AsyncWebServer server(80);
const char* SSID = "Peachy2.4";
const char* PASSWORD = "Friendly";
OneWire glycol(16);
DS18B20 glycolSen(&glycol);



void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // WiFi.begin(SSID, PASSWORD);
  // while(WiFi.status() != WL_CONNECTED){
  //   delay(1000);
  //   Serial.println("Connecting to WiFi..");
  // }
  // Serial.printf("Connected to the WiFi network with IP Address: %s\n", WiFi.localIP().toString().c_str());
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/index.html");
  // });
  // server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", "50");
  // });
  // server.begin();
}

void loop() {
  Serial.println("Regquesting Sensor Temperature");
  glycolSen.requestTemperatures();
  while(!glycolSen.isConversionComplete());
  Serial.printf("Sensor Temperature: %f\n", glycolSen.getTempC());
  delay(5000);
}