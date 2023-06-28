#include <Sensors.h>
#include <numeric>

extern tm timeData;
tempProbe::tempProbe(const uint8_t *address)
{
    for (auto index : uniqueAddress)
    {
        index = *address;
        address++;
    }
    realTime.fill(0);
    hourly.fill(0);
    daily.fill(0);
    monthly.fill(0);
    anually.fill(0);
}

OneWire tempProbe::oneWire(ONE_WIRE_BUS);
DallasTemperature tempProbe::sensors(&oneWire);
std::array<tempProbe, 5> tempProbe::probes = {tempProbe(GLYCOL_ADDR), tempProbe(PREHEAT_ADDR), tempProbe(AMBIENT_ADDR), tempProbe(SOURCE_ADDR), tempProbe(HOT_ADDR)};
short tempProbe::indexRealTime = 0, tempProbe::indexHourly = 0, tempProbe::indexDaily = 0, tempProbe::indexMonthly = 0, tempProbe::indexAnually = 0;
bool tempProbe::updateHourly = false, tempProbe::updateDaily = false, tempProbe::updateMonthly = false, tempProbe::updateAnually = false;

void tempProbe::readAllProbes()
{
    sensors.requestTemperatures();
    for (auto &probe : probes) probe.realTime.at(indexRealTime) = static_cast<short>(sensors.getTempC(probe.uniqueAddress));
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
            probe.hourly.at(indexHourly) = static_cast<short>(std::accumulate(probe.realTime.begin(), probe.realTime.end(), 0.0) / probe.realTime.size());
            probe.realTime.fill(0);
        }
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
            probe.hourly.fill(0);
            updateCSV();
        }
        indexDaily++;
        if(indexDaily == 24)
        {
            indexDaily = 0;
            updateMonthly = true;
        }
        updateDaily = false;
    }
    if (updateMonthly)
    {
        for(auto& probe : probes)
        {
            probe.monthly.at(indexMonthly) = static_cast<short>(std::accumulate(probe.daily.begin(), probe.daily.end(), 0.0) / probe.daily.size());
            probe.daily.fill(0);
        }
        indexMonthly++;
        if(indexMonthly == 31)
        {
            indexMonthly = 0;
            updateAnually = true;
        }
        updateMonthly = false;
    }
    if (updateAnually)
    {
        for(auto& probe : probes)
        {
            probe.anually.at(indexAnually) = static_cast<short>(std::accumulate(probe.monthly.begin(), probe.monthly.end(), 0.0) / probe.monthly.size());
            probe.monthly.fill(0);
        }
        indexAnually++;
        if(indexAnually == 12)
        {
            indexAnually = 0;
        }
        updateAnually = false;
    }
}

String tempProbe::getRealTimeData()
{
    String data = "";
    for(auto &probe : probes)
    {
        data += String(static_cast<float>(probe.realTime.at(indexRealTime))/100.0) + ",";
        
    }
    data.remove(data.length() - 1); //remove the final comma
    return data;
}

String tempProbe::getHourlyData()
{
    String data = "";
    for(auto &probe : probes)
    {
        data += String(static_cast<float>(probe.hourly.at(indexHourly))/100.0) + ",";
    }
    data.remove(data.length() - 1); //remove the final comma
    return data;
}

String tempProbe::getHistoricalData()
{

}

void tempProbe::updateCSV()
{
    char buffer[50];
    if(!getLocalTime(&timeData)) Serial.println("Failed to obtain time");
    //Add timestamp to data in the form YYYY-Month-DD HH:MM:SS
    sprintf(buffer, "%Y-%B-%d %H:%M", &timeData);
    String timestamp = buffer;
    timestamp.trim();
    String data = "";
    data += timestamp + ",";
    for(auto &probe : probes)
    {
        data += String(static_cast<float>(probe.daily.at(indexDaily))/100.0) + ",";
    }
    data.remove(data.length() - 1); //remove the final comma
    File file = SPIFFS.open("/historical_data.csv", FILE_APPEND);
    file.println(data);
    file.close();
}