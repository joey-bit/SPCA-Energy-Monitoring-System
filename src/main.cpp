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
const char* STORED_SSID = "Peachy5.0";
const char* STORED_PASSWORD = "Friendly";

//Time information
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -28800;
const int daylightOffset_sec = 3600;

//Variables used to update wifi credentials
String inputSSID;
String inputPass;
bool updateWIFI = false;

//Static IP configuration for access point mode
IPAddress ap_IP(192, 168, 1, 1);
IPAddress ap_subnet(255, 255, 255, 0);
IPAddress ap_gateway(192, 168, 1, 1);

//Stores timedata and updates automatically when time is first fetched
struct tm timeData;

//Function declaration
bool connectWIFI(const char* ssid, const char* pass);

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
  WiFi.softAPConfig(ap_IP, ap_gateway, ap_subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  connectWIFI(STORED_SSID, STORED_PASSWORD);

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
  server.on("/wifi-creds", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("ssid")) {
      inputSSID = request->getParam("ssid")->value();
      inputPass = request->getParam("pass")->value();
      updateWIFI = true;
    }
    request->send_P(200, "text/plain", "Connecting, please wait 15 seconds and then enter 192.168.1.1 in the address bar");
  });
  server.on("/wifi-stat", HTTP_GET, [](AsyncWebServerRequest *request){
    String details;
    if(WiFi.status() == WL_CONNECTED)
    {
      details += "Connected to ";
      details += WiFi.SSID();
      details += " with IP Address: ";
      details += WiFi.localIP().toString();
    }
    else
    {
      details += "Not connected to a network";
    }
    request->send_P(200, "text/plain", details.c_str());
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
  if(updateWIFI)
  {
    updateWIFI = false;
    connectWIFI(inputSSID.c_str(), inputPass.c_str());
  }
}

bool connectWIFI(const char* ssid, const char* pass) {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  for(auto i = 0; i<5; i++){
    if(WiFi.status() == WL_CONNECTED){
      Serial.println();
      Serial.printf("Connected to the WiFi network with IP Address: %s\n", WiFi.localIP().toString().c_str());
      digitalWrite(LED_PIN, HIGH);
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      delay(500);
    if(!getLocalTime(&timeData)){
      Serial.println("Failed to obtain time");
    } else {
      Serial.printf("Current time: %s\n", asctime(&timeData));
    } 
      return true;
    }
    delay(3000);
    Serial.print(".");
  }
  if(WiFi.status() != WL_CONNECTED)
    Serial.println("Failed to connect to the WiFi network, operating in AP mode");
  return false;
}