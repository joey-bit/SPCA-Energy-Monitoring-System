#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <Sensors.h>
#include <time.h>

AsyncWebServer server(80);
const char* SSID = "SCCSA";
const char* PASSWORD = "sccsa2023";
const char* AP_SSID = "Hot Water Monitor";
const char* AP_PASSWORD = "solarpower";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -28800;
const int daylightOffset_sec = 3600;

IPAddress IP(192, 168, 1, 1);
IPAddress local_IP(192, 168, 1, 117); // Desired Static IP Address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

tm timeData;

DeviceAddress addr;

#define LED_PIN 2
bool connectWIFI();
bool reconnect = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  delay(500);
  WiFi.softAPConfig(IP, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  connectWIFI();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.serveStatic("/", SPIFFS, "/");
  server.on("/temperature-rt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", tempProbe::getRealTimeTemp().c_str());
  });
  server.on("/temperature-hr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", tempProbe::getHourlyTemp().c_str());
  });
  server.on("/flow-rt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", flowMeter::instance.getRealTimeFlow().c_str());
  });
  server.on("/flow-hr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", flowMeter::instance.getHourlyFlow().c_str());
  });
  server.on("/historical-data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/historical_data.csv");
  });
  server.on("/energy-rt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", tempProbe::getRealTimePower().c_str());
  });
  server.on("/energy-hr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", tempProbe::getHourlyEnergy().c_str());
  });
  server.on("/wifi-reconnect", HTTP_GET, [](AsyncWebServerRequest *request){
    if(WiFi.status() != WL_CONNECTED)
      reconnect = true;
    request->send_P(200, "text/plain", "connecting");
  });

  server.begin();
  pinMode(FLOW_METER_PIN, INPUT_PULLUP);
  tempProbe::sensors.begin();
  printf("Found %d sensors\n", tempProbe::sensors.getDeviceCount());
}

void loop() {
  // put your main code here, to run repeatedly:
  tempProbe::readAllProbes();
  delay(5000);  //Actually occurs every 6 seconds because readFlowMeter takes 1 second
  if(reconnect){
    connectWIFI();
  }
}

bool connectWIFI() {
  Serial.print("Connecting to WiFi");
  if (!WiFi.config(local_IP, gateway, subnet, dns))
  {
    Serial.println("Configuration Failed!");
  }
  WiFi.begin(SSID, PASSWORD);
  for(auto i = 0; i<5; i++){
    if(WiFi.status() == WL_CONNECTED){
      Serial.println();
      Serial.printf("Connected to the WiFi network with IP Address: %s\n", WiFi.localIP().toString().c_str());
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      delay(500);
    if(!getLocalTime(&timeData)){
      Serial.println("Failed to obtain time");
    } else {
      Serial.printf("Current time: %s\n", asctime(&timeData));
      digitalWrite(LED_PIN, HIGH);
      return true;
    } 
    }
    delay(3000);
    Serial.print(".");
  }
  if(WiFi.status() != WL_CONNECTED)
    Serial.println("Failed to connect to the WiFi network, operating in AP mode only");
  return false;
}
