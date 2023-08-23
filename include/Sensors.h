#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPIFFS.h>
#include <map>

#define ONE_WIRE_BUS 15
#define FLOW_METER_PIN 14

static const uint8_t GLYCOL_ADDR[8] = {0x28, 0x59, 0x5C, 0x97, 0x94, 0x12, 0x03, 0x5C};
static const uint8_t PREHEAT_ADDR[8] = {0x28, 0x02, 0x50, 0x97, 0x94, 0x05, 0x03, 0x29};
static const uint8_t AMBIENT_ADDR[8] = {0x28, 0x4A, 0x02, 0x97, 0x94, 0x05, 0x03, 0xEB};
static const uint8_t SOURCE_ADDR[8] = {0x28, 0x69, 0x36, 0x97, 0x94, 0x12, 0x03, 0x1D};
static const uint8_t HOT_ADDR[8] = {0x28, 0x29, 0x57, 0x97, 0x94, 0x12, 0x03, 0xB4};

typedef struct tempProbe {
    tempProbe(const uint8_t* address);
       
    std::array<short , 20> realTime;
    std::array<short , 30> hourly;
    std::array<short , 24> daily;

    const uint8_t* uniqueAddress;

    static short indexRealTime, indexHourly, indexDaily;
    static bool updateHourly, updateDaily;

    static void readAllProbes();
    static String getRealTimeTemp();
    static String getHourlyTemp();
    static String getRealTimePower();
    static String getHourlyEnergy();
    static void updateCSV();
    static OneWire oneWire;
    static DallasTemperature sensors;
    static std::array<tempProbe, 5> probes; //Order of probes folllows the order of the enum above
}tempProbe;

typedef struct flowMeter {
    flowMeter();
    uint16_t pulses;
    static flowMeter instance;
    void readFlowMeter();
    static String getRealTimeFlow();
    static String getHourlyFlow();
    std::array<short , 20> realTime;    //Stored in L/min
    std::array<short , 30> hourly;      //Integration of rate, stored in L
    std::array<short , 24> daily;       //Integration of rate, stored in L
}flowMeter;