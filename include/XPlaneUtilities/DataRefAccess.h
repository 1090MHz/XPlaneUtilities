#ifndef DATAREFACCESS_H
#define DATAREFACCESS_H

#include <XPLMDataAccess.h>
#include <stdexcept>
#include <string>

namespace XPlaneUtilities {

/**
 * DataRefAccess - Read and write existing X-Plane datarefs
 *
 * This template class caches the X-Plane dataref handle on construction and
 * exposes typed read/write access for datarefs owned by X-Plane or other
 * plugins. Unlike DataRefExport, this class does not register a new dataref.
 *
 * Example usage:
 *   DataRefAccess<int> beaconOn("sim/cockpit/electrical/beacon_lights_on");
 *   beaconOn = 1;
 *
 *   DataRefAccess<float> baroSetting("sim/cockpit/misc/barometer_setting", 29.92f);
 *   float baro = baroSetting.get();
 */
template<typename T>
class DataRefAccess final
{
public:
    DataRefAccess(const std::string &name);
    DataRefAccess(const std::string &name, T defaultValue);

    operator T() const;
    T get() const;
    bool set(T value);
    DataRefAccess& operator=(T value);

    bool isValid() const { return handle != nullptr; }
    bool isWritable() const { return writable; }
    XPLMDataTypeID getTypes() const { return supportedTypes; }

private:
    std::string dataRefName;
    T overrideValue{};
    XPLMDataRef handle = nullptr;
    XPLMDataTypeID supportedTypes = xplmType_Unknown;
    bool writable = false;
    bool compatibleType = false;
};

} // namespace XPlaneUtilities

#endif // DATAREFACCESS_H