#include <Sensors.h>
#include <numeric>

extern tm timeData;

tempProbe::tempProbe(const uint8_t* address)
{
    uniqueAddress = address;
    realTime.fill(0);
    hourly.fill(0);
    daily.fill(0);
}

OneWire tempProbe::oneWire(ONE_WIRE_BUS);
DallasTemperature tempProbe::sensors(&oneWire);
std::array<tempProbe, 5> tempProbe::probes = {tempProbe(GLYCOL_ADDR), tempProbe(PREHEAT_ADDR), tempProbe(AMBIENT_ADDR), tempProbe(SOURCE_ADDR), tempProbe(HOT_ADDR)};
short tempProbe::indexRealTime = 0, tempProbe::indexHourly = 0, tempProbe::indexDaily = 0;
bool tempProbe::updateHourly = false, tempProbe::updateDaily = false;

void tempProbe::readAllProbes()
{
    sensors.requestTemperatures();
    for (auto &probe : probes){
        auto temp = probe.sensors.getTempC(probe.uniqueAddress);
        probe.realTime[indexRealTime] = static_cast<short>(temp*100);
        //Serial.printf("Reading %.2f, storing %d at index %d\n", temp, probe.realTime[indexRealTime], indexRealTime);
    } 
    flowMeter::instance.readFlowMeter();
    indexRealTime++;  
    if(indexRealTime == 20) 
    {
        indexRealTime = 0;
        updateHourly = true;
    }
    if (updateHourly)
    {
        for(auto &probe : probes) 
        {
            probe.hourly[indexHourly] = static_cast<short>(std::accumulate(probe.realTime.begin(), probe.realTime.end(), 0.0) / probe.realTime.size());
        }
        flowMeter::instance.hourly[indexHourly] = static_cast<short>(std::accumulate(flowMeter::instance.realTime.begin(), flowMeter::instance.realTime.end(), 0.0)/10.0);
        indexHourly++;
        if(indexHourly == 60)
        {
            indexHourly = 0;
            updateDaily = true;
        } 
        updateHourly = false;
    }
    if (updateDaily)
    {
        for(auto &probe : probes)
        {
            probe.daily.at(indexDaily) = static_cast<short>(std::accumulate(probe.hourly.begin(), probe.hourly.end(), 0.0) / probe.hourly.size());
        }
        flowMeter::instance.daily[indexDaily] = static_cast<short>(std::accumulate(flowMeter::instance.hourly.begin(), flowMeter::instance.hourly.end(), 0.0));
        updateCSV();
        indexDaily++;
        if(indexDaily == 24)
        {
            indexDaily = 0;
        }
        updateDaily = false;
    }
}

String tempProbe::getRealTimeData()
{
    String data = "";
    for(auto &probe : probes)
    {
        try{
            data += String(probe.realTime.at(indexRealTime-1)) + ",";
        }
        catch(const std::out_of_range& oor)
        {
            data += String(probe.realTime.at(19)) + ",";    //We are on index 0, so we want the last element of the array
        }
        
    }
    data.remove(data.length() - 1); //remove the final comma
    printf("Sending to HTML: %s\n", data.c_str());
    return data;
}

String tempProbe::getHourlyData()
{
    String data = "";
    for(auto &probe : probes)
    {
        try{
            data += String(probe.hourly.at(indexHourly-1)) + ",";
        }
        catch(const std::out_of_range& oor)
        {
            data += String(probe.hourly.at(19)) + ",";   //We are on index 0, so we want the last element of the array
        }
    }
    data.remove(data.length() - 1); //remove the final comma
    return data;
}

void tempProbe::updateCSV()
{
    if(!getLocalTime(&timeData)) Serial.println("Failed to obtain time");
    String data = "";
    //Add timestamp to data in the form YYYY-Month-DD HH:MM:SS
    data += String(timeData.tm_year + 1900) + "-";
    if(timeData.tm_mon < 10) data += "0";
    data += String(timeData.tm_mon + 1) + "-";
    if(timeData.tm_mday < 10) data += "0";
    data += String(timeData.tm_mday) + " ";
    if(timeData.tm_hour < 10) data += "0";
    data += String(timeData.tm_hour) + ":";
    if(timeData.tm_min < 10) data += "0";
    data += String(timeData.tm_min) + ":";
    if(timeData.tm_sec < 10) data += "0";
    data += String(timeData.tm_sec);
    data += ",";
    for(auto &probe : probes)
    {
        data += String(static_cast<float>(probe.daily.at(indexDaily))/100.0) + ",";
    }
    data += String(static_cast<float>(flowMeter::instance.daily.at(indexDaily))/100.0);
    auto fileHandle = SPIFFS.open("/historical_data.csv", FILE_APPEND);
    if(!fileHandle)
    { 
        Serial.println("Failed to open file for appending");
        return;
    }
    if(!fileHandle.println(data)) Serial.println("Failed to append file");
    else Serial.println("File appended");
    fileHandle.close();
    
}

flowMeter flowMeter::instance{};
flowMeter::flowMeter()
{
    pulses = 0;
    realTime.fill(0);
    hourly.fill(0);
    daily.fill(0);
}

void IRAM_ATTR pulseCounter()
{
    flowMeter::instance.pulses++;
}

void flowMeter::readFlowMeter() {
    instance.pulses = 0;
    attachInterrupt(FLOW_METER_PIN, pulseCounter, FALLING);
    delay(1000);
    detachInterrupt(FLOW_METER_PIN);
    //Serial.printf("Pulses: %d\n", instance.pulses);
    instance.realTime.at(tempProbe::indexRealTime) = static_cast<short>(instance.pulses/ 5.5*100);
    //Serial.printf("Flow meter reading: %.3f L/min\n", static_cast<float>(instance.realTime.at(tempProbe::indexRealTime))/100.0);
}