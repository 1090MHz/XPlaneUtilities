#include <XPlaneUtilities/FlightDataProvider.h>
#include <cstdio>

namespace XPlaneUtilities {

FlightDataProvider::FlightDataProvider()
{
    // Initialize all dataref imports (AviTab pattern - cache handles on construction)
    // Using defaults of 0 ensures graceful degradation if datarefs are missing
    
    m_fuelTotal = std::make_unique<DataRefImport<float>>(
        "sim/flightmodel/weight/m_fuel_total", 0.0f);
    
    m_zuluTime = std::make_unique<DataRefImport<float>>(
        "sim/time/zulu_time_sec", 0.0f);
    
    m_groundSpeed = std::make_unique<DataRefImport<float>>(
        "sim/flightmodel/position/groundspeed", 0.0f);
    
    m_latitude = std::make_unique<DataRefImport<double>>(
        "sim/flightmodel/position/latitude", 0.0);
    
    m_longitude = std::make_unique<DataRefImport<double>>(
        "sim/flightmodel/position/longitude", 0.0);
    
    m_airspeed = std::make_unique<DataRefImport<float>>(
        "sim/flightmodel/position/indicated_airspeed", 0.0f);
    
    m_elevation = std::make_unique<DataRefImport<double>>(
        "sim/flightmodel/position/elevation", 0.0);
}

//==========================================================================
// Read X-Plane State (efficient - uses cached handles)
//==========================================================================

float FlightDataProvider::getFuelOnBoard()
{
    return *m_fuelTotal;  // Conversion operator automatically calls XPLMGetDataf
}

float FlightDataProvider::getZuluTimeSec()
{
    return *m_zuluTime;
}

float FlightDataProvider::getGroundSpeed()
{
    return *m_groundSpeed;
}

double FlightDataProvider::getLatitude()
{
    return *m_latitude;
}

double FlightDataProvider::getLongitude()
{
    return *m_longitude;
}

float FlightDataProvider::getIndicatedAirspeed()
{
    return *m_airspeed;
}

double FlightDataProvider::getElevation()
{
    return *m_elevation;
}

//==========================================================================
// Utility Functions
//==========================================================================

std::string FlightDataProvider::formatTimeHHMM(float seconds)
{
    int totalSeconds = static_cast<int>(seconds);
    int hours = (totalSeconds / 3600) % 24;
    int minutes = (totalSeconds % 3600) / 60;
    
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%02d%02d", hours, minutes);
    return std::string(buffer);
}

std::string FlightDataProvider::getZuluTimeFormatted()
{
    float seconds = getZuluTimeSec();
    return formatTimeHHMM(seconds);
}

} // namespace XPlaneUtilities
