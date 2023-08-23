#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPIFFS.h>
#include <map>

#define ONE_WIRE_BUS 15
#define FLOW_METER_PIN 14

// Addresses of the temperature probes
static const uint8_t GLYCOL_ADDR[8] = {0x28, 0x34, 0x41, 0x97, 0x94, 0x12, 0x03, 0x45}; 
static const uint8_t PREHEAT_ADDR[8] = {0x28, 0x9A, 0x4B, 0x97, 0x94, 0x12, 0x03, 0xB7};
static const uint8_t AMBIENT_ADDR[8] = {0x28, 0xB6, 0x02, 0x97, 0x94, 0x05, 0x03, 0xE4};
static const uint8_t SOURCE_ADDR[8] = {0x28, 0x96, 0x3B, 0x97, 0x94, 0x10, 0x03, 0x36}; 
static const uint8_t HOT_ADDR[8] = {0x28, 0xD8, 0x61, 0x97, 0x94, 0x12, 0x03, 0xE9};

/*
** This struct is used to store the data from an individual temperature probe. Static variables and functions can be accessed by any instance of the struct.
** Only the arrays and unique address are unique to each instance.
*/
typedef struct tempProbe {
    tempProbe(const uint8_t* address);  //Constructor
    
    //Arrays that store the data
    std::array<short , 20> realTime;    //6 seconds per element
    std::array<short , 30> hourly;      //2 minutes per element
    std::array<short , 24> daily;       //1 hour per element

    const uint8_t* uniqueAddress;       //Stores the unique address of the probe 

    //Keeps track of the index of the data arrays
    static short indexRealTime, indexHourly, indexDaily;

    //Functions that update the data arrays
    static void readAllProbes();
    static void incrementRealTime();
    static void updateHourlyData();
    static void updateDailyData();

    //Functions that extract strings from the data to update the website, and write to the historical data file
    static String getRealTimeTemp();
    static String getHourlyTemp();
    static String getRealTimePower();
    static String getHourlyEnergy();
    static void updateCSV();

    //Objects required to interface with temp sensors
    static OneWire oneWire;
    static DallasTemperature sensors;

    //Array containing all 5 probe objects, makes it easy to interact with all 5 simultaneously
    static std::array<tempProbe, 5> probes; 
}tempProbe;

/*
** This struct is used to store the data from the flow meter. Static variables and functions can be accessed by any instance of the struct.
*/
typedef struct flowMeter {
    flowMeter();                        //Constructor
    uint16_t pulses;                    //Counts pulses from flow meter in 1 second
    static flowMeter instance;          //Only one instance of flowMeter is needed
    void readFlowMeter();               //Reads the flow meter and updates the realTime array
    static String getRealTimeFlow();    //Extracts a string from the realTime array to update the website
    static String getHourlyFlow();      //Extracts a string from the hourly array to update the website
    std::array<short , 20> realTime;    //Stored in L/min at 6 seconds per element
    std::array<short , 30> hourly;      //Integration of rate, stored in L at 2 minutes per element
    std::array<short , 24> daily;       //Integration of rate, stored in L at 1 hour per element
}flowMeter;