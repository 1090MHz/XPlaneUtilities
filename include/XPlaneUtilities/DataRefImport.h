#ifndef DATAREFIMPORT_H
#define DATAREFIMPORT_H

#include <XPLMDataAccess.h>
#include <string>
#include <stdexcept>

namespace XPlaneUtilities {

/**
 * DataRefImport - Read datarefs from X-Plane
 * 
 * Based on AviTab's implementation, this template class simplifies
 * reading X-Plane datarefs with type safety and automatic conversion.
 * 
 * The dataref handle is cached on construction, making repeated reads efficient.
 * 
 * Example usage:
 *   DataRefImport<float> fuelTotal("sim/flightmodel/weight/m_fuel_total");
 *   float fuel = fuelTotal;  // Automatically reads current value
 * 
 *   DataRefImport<int> gearDeploy("sim/flightmodel/movingparts/gear_deploy", 0);
 *   int gear = gearDeploy;  // Uses default if dataref not found
 */
template<typename T>
class DataRefImport final
{
public:
    // Constructor - throws if dataref not found
    DataRefImport(const std::string &name);
    
    // Constructor with default value - uses default if dataref not found
    DataRefImport(const std::string &name, T defaultValue);
    
    // Conversion operator - allows using object like native type
    operator T();
    
    // Check if dataref is valid
    bool isValid() const { return handle != nullptr; }

private:
    T overrideValue{};
    XPLMDataRef handle = nullptr;
};

} // namespace XPlaneUtilities

#endif // DATAREFIMPORT_H
