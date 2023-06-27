#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <Sensors.h>
#include <time.h>

AsyncWebServer server(80);
const char* SSID = "Peachy2.4";
const char* PASSWORD = "Friendly";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -28800;
const int daylightOffset_sec = 3600;

tm timeData;

void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.printf("Connected to the WiFi network with IP Address: %s\n", WiFi.localIP().toString().c_str());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeData)){
    Serial.println("Failed to obtain time");
    return;
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature-rt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/csv", tempProbe::getRealTimeData().c_str());
  });
  server.on("/temperature-hr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/csv", tempProbe::getHourlyData().c_str());
  });
  server.on("/temperature-hist", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/csv", tempProbe::getHistoricalData().c_str());
  });
  server.on("/energy-rt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/csv", "100,200");
  });
  server.on("/flow-rt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "8.4");
  });
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  tempProbe::readAllProbes();
  delay(6000);
}