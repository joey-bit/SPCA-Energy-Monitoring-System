#include <Sensors.h>
#include <numeric>

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
std::array<tempProbe, 5> tempProbe::probes = {tempProbe(GLYCOL_ADDR), tempProbe(PREHEAT_ADDR), tempProbe(HYBRID_ADDR), tempProbe(SOURCE_ADDR), tempProbe(HOT_ADDR)};

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
