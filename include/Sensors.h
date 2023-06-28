#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPIFFS.h>

#define ONE_WIRE_BUS 15
#define FLOW_METER_PIN 14

const uint8_t GLYCOL_ADDR[8] = {0x28, 0x59, 0x5C, 0x97, 0x94, 0x12, 0x03, 0x5C};
const uint8_t PREHEAT_ADDR[8] = {0x28, 0x02, 0x50, 0x97, 0x94, 0x05, 0x03, 0x29};
const uint8_t AMBIENT_ADDR[8] = {0x28, 0x4A, 0x02, 0x97, 0x94, 0x05, 0x03, 0xEB};
const uint8_t SOURCE_ADDR[8] = {0x28, 0x69, 0x36, 0x97, 0x94, 0x12, 0x03, 0x1D};
const uint8_t HOT_ADDR[8] = {0x28, 0x29, 0x57, 0x97, 0x94, 0x12, 0x03, 0xB4};

typedef struct tempProbe {
    tempProbe(const uint8_t* address);
       
    std::array<short , 20> realTime;
    std::array<short , 30> hourly;
    std::array<short , 24> daily;
    std::array<short , 31> monthly;
    std::array<short , 12> anually;

    uint8_t uniqueAddress[8];

    static short indexRealTime, indexHourly, indexDaily, indexMonthly, indexAnually;
    static bool updateHourly, updateDaily, updateMonthly, updateAnually;

    static void readAllProbes();
    static String getRealTimeData();
    static String getHourlyData();
    static String getHistoricalData();
    static void updateCSV();
    static OneWire oneWire;
    static DallasTemperature sensors;
    static std::array<tempProbe, 5> probes; //Order of probes folllows the order of the enum above
}tempProbe;

typedef struct flowMeter {
    uint16_t pulses;
    float flowRate;
    static flowMeter instance;
    void readFlowMeter();
}flowMeter;