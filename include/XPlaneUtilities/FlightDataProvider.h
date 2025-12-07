#ifndef FLIGHTDATAPROVIDER_H
#define FLIGHTDATAPROVIDER_H

/*
 *   XPlaneUtilities - Flight Data Provider using AviTab patterns
 *   
 *   Provides both reading (DataRefImport) and writing (DataRefExport) capabilities
 *   for X-Plane datarefs using proven, battle-tested patterns from AviTab.
 */

#include "DataRefImport.h"
#include "DataRefExport.h"
#include <string>
#include <memory>

namespace XPlaneUtilities {

/**
 * @brief Flight data provider using AviTab's proven dataref patterns
 * 
 * This class demonstrates best practices for X-Plane dataref management:
 * - DataRefImport: Read X-Plane state (cached handles, efficient)
 * - DataRefExport: Expose plugin state to other plugins/tools
 */
class FlightDataProvider
{
public:
    FlightDataProvider();
    ~FlightDataProvider() = default;

    // Prevent copying (datarefs should not be copied)
    FlightDataProvider(const FlightDataProvider&) = delete;
    FlightDataProvider& operator=(const FlightDataProvider&) = delete;

    //==========================================================================
    // Read X-Plane State (DataRefImport)
    //==========================================================================

    /**
     * @brief Get current fuel on board in kilograms
     * @return Fuel weight in kg
     */
    float getFuelOnBoard();

    /**
     * @brief Get current Zulu/UTC time in seconds since midnight
     * @return Time in seconds (0-86400)
     */
    float getZuluTimeSec();

    /**
     * @brief Get current ground speed in meters per second
     * @return Ground speed in m/s
     */
    float getGroundSpeed();

    /**
     * @brief Get current latitude
     * @return Latitude in degrees
     */
    double getLatitude();

    /**
     * @brief Get current longitude
     * @return Longitude in degrees
     */
    double getLongitude();

    /**
     * @brief Get current indicated airspeed in knots
     * @return Airspeed in knots
     */
    float getIndicatedAirspeed();

    /**
     * @brief Get current elevation (altitude MSL) in feet
     * @return Elevation in feet MSL
     */
    double getElevation();

    //==========================================================================
    // Utility Functions
    //==========================================================================

    /**
     * @brief Convert seconds since midnight to HHMM format string
     * @param seconds Time in seconds since midnight
     * @return Time string in HHMM format (e.g., "1430" for 14:30)
     */
    static std::string formatTimeHHMM(float seconds);

    /**
     * @brief Get current Zulu time formatted as HHMM
     * @return Time string (e.g., "1430")
     */
    std::string getZuluTimeFormatted();

    /**
     * @brief Convert fuel from kg to tons
     */
    static float kgToTons(float kg) { return kg / 1000.0f; }

    /**
     * @brief Convert fuel from kg to pounds
     */
    static float kgToLbs(float kg) { return kg * 2.20462f; }

    /**
     * @brief Convert meters per second to knots
     */
    static float msToKnots(float ms) { return ms * 1.94384f; }

private:
    // X-Plane dataref imports (AviTab pattern - cached handles)
    std::unique_ptr<DataRefImport<float>> m_fuelTotal;
    std::unique_ptr<DataRefImport<float>> m_zuluTime;
    std::unique_ptr<DataRefImport<float>> m_groundSpeed;
    std::unique_ptr<DataRefImport<double>> m_latitude;
    std::unique_ptr<DataRefImport<double>> m_longitude;
    std::unique_ptr<DataRefImport<float>> m_airspeed;
    std::unique_ptr<DataRefImport<double>> m_elevation;
};

} // namespace XPlaneUtilities

#endif // FLIGHTDATAPROVIDER_H
