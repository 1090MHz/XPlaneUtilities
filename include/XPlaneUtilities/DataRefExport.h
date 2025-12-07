#ifndef DATAREFEXPORT_H
#define DATAREFEXPORT_H

#include <XPLMDataAccess.h>
#include <string>
#include <functional>

namespace XPlaneUtilities {

/**
 * DataRefExport - Export custom datarefs to X-Plane
 * 
 * Based on AviTab's implementation, this template class simplifies
 * the creation of custom datarefs that other plugins or systems can read.
 * 
 * Supports read-only and read-write datarefs for int and float types.
 * 
 * Example usage:
 *   auto myDataRef = std::make_unique<DataRefExport<int>>(
 *       "simbreviloquent/flight_plan/loaded",
 *       this,
 *       [](void *ref) { return static_cast<MyClass*>(ref)->isLoaded() ? 1 : 0; }
 *   );
 */
template<typename T>
class DataRefExport final
{
public:
    // Read-only dataref
    DataRefExport(const std::string &name, void *owner, std::function<T(void *)> onRead);
    
    // Read-write dataref
    DataRefExport(const std::string &name, void *owner, 
                  std::function<T(void *)> onRead,
                  std::function<void(void *, T)> onWrite);
    
    ~DataRefExport();

    // Prevent copying
    DataRefExport(const DataRefExport&) = delete;
    DataRefExport& operator=(const DataRefExport&) = delete;

private:
    void *ownerRef;
    std::function<T(void *)> onRead;
    std::function<void(void *, T)> onWrite;
    XPLMDataRef xpDataRef = nullptr;
};

} // namespace XPlaneUtilities

#endif // DATAREFEXPORT_H
