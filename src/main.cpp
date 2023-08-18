#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <Sensors.h>
#include <time.h>

#define LED_PIN 2

//Server object and wifi/time information
AsyncWebServer server(80);
const char* AP_SSID = "Hot Water Monitor";
const char* AP_PASSWORD = "solarpower";
const char* SSID = "Peachy2.4";
const char* PASSWORD = "Friendly";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -28800;
const int daylightOffset_sec = 3600;

// Desired Static IP configuration, access the web server at this address (you can set this to whatever 192.168.1.XXX you want)
IPAddress local_IP(192, 168, 1, 117); 
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//Stores timedata and updates automatically when time is first fetched
tm timeData;

//Function declaration
bool connectWIFI();

//Runs once at startup
void setup() {
  //Initialize serial communication for laptop terminal
  Serial.begin(115200);

  //Turn LED off to begin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  //Mount the SPIFFS file system
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //Configure the WiFi
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  delay(500);
  WiFi.softAPConfig(gateway, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  connectWIFI();

  //Callback functions of the web server
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
  server.on("/wifi-reconnect", HTTP_GET, [](AsyncWebServerRequest *request){
    if(WiFi.status() != WL_CONNECTED)
      connectWIFI();
    request->send_P(200, "text/plain", "connected");
  });

  //Start the web server
  server.begin();

  //Initialize the temperature probes and flow meter
  pinMode(FLOW_METER_PIN, INPUT_PULLUP);
  tempProbe::sensors.begin();
  printf("Found %d sensors\n", tempProbe::sensors.getDeviceCount());
}

void loop() {
  tempProbe::readAllProbes();
  delay(5000);  //Actually occurs every 6 seconds because readFlowMeter takes 1 second
}

bool connectWIFI() {
  Serial.print("Connecting to WiFi");
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("Configuration Failed!");
  }
  WiFi.begin(SSID, PASSWORD);
  for(auto i = 0; i<5; i++){
    delay(3000);
    if(WiFi.status() == WL_CONNECTED){
      Serial.println();
      Serial.printf("Connected to the WiFi network with IP Address: %s\n", WiFi.localIP().toString().c_str());
      digitalWrite(LED_PIN, HIGH);
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if(!getLocalTime(&timeData)){
      Serial.println("Failed to obtain time");
    } else {
      Serial.printf("Current time: %s\n", asctime(&timeData));
    } 
      return true;
    }
    Serial.print(".");
  }
  if(WiFi.status() != WL_CONNECTED)
    Serial.println("Failed to connect to the WiFi network, operating in AP mode");
  return false;
}