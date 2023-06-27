#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 15

const uint8_t GLYCOL_ADDR[8] = {0x28, 0x3C, 0x7D, 0x7B, 0x07, 0x00, 0x00, 0x9A};
const uint8_t PREHEAT_ADDR[8] = {0x28, 0x3C, 0x7D, 0x7B, 0x07, 0x00, 0x00, 0x9A};
const uint8_t HYBRID_ADDR[8] = {0x28, 0x3C, 0x7D, 0x7B, 0x07, 0x00, 0x00, 0x9A};
const uint8_t SOURCE_ADDR[8] = {0x28, 0x3C, 0x7D, 0x7B, 0x07, 0x00, 0x00, 0x9A};
const uint8_t HOT_ADDR[8] = {0x28, 0x3C, 0x7D, 0x7B, 0x07, 0x00, 0x00, 0x9A};

enum probeType
{
    GLYCOL,
    PREHEAT,
    HYBRID,
    SOURCE,
    HOT
};

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