#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <Sensors.h>
#include <time.h>

AsyncWebServer server(80);
const char* SSID = "TELUS9344";
const char* PASSWORD = "fz4mz6yrzf";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -28800;
const int daylightOffset_sec = 3600;

tm timeData;

DeviceAddress addr;

void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(5000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.printf("Connected to the WiFi network with IP Address: %s\n", WiFi.localIP().toString().c_str());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeData)){
    Serial.println("Failed to obtain time");
  } else {
    Serial.printf("Current time: %s\n", asctime(&timeData));
  } 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.serveStatic("/", SPIFFS, "/");
  server.on("/temperature-rt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", tempProbe::getRealTimeData().c_str());
  });
  server.on("/temperature-hr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", tempProbe::getHourlyData().c_str());
  });
  server.on("/temperature-day", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/historical_data.csv");
  });
  // server.on("/energy-rt", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/csv", "100,200");
  // });
  // server.on("/flow-rt", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", "8.4");
  //});
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //tempProbe::readAllProbes();
  //delay(6000);
}