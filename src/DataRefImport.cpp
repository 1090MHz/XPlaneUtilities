#include <XPlaneUtilities/DataRefImport.h>
#include <XPlaneUtilities/XPlaneLog.h>

namespace XPlaneUtilities {

// Constructor - throws if dataref not found
template<typename T>
DataRefImport<T>::DataRefImport(const std::string &name)
{
    handle = XPLMFindDataRef(name.c_str());
    if (!handle) {
        throw std::runtime_error("Invalid DataRef: " + name);
    }
}

// Constructor with default value - uses default if dataref not found
template<typename T>
DataRefImport<T>::DataRefImport(const std::string &name, T defaultValue)
{
    handle = XPLMFindDataRef(name.c_str());
    if (!handle) {
        XPlaneLog::warn("DataRef '" + name + "' not available, using default value");
        overrideValue = defaultValue;
    }
}

// Template specialization for int type
template<>
DataRefImport<int>::operator int()
{
    if (handle) {
        return XPLMGetDatai(handle);
    } else {
        return overrideValue;
    }
}

// Template specialization for bool type
template<>
DataRefImport<bool>::operator bool()
{
    if (handle) {
        return XPLMGetDatai(handle) != 0;
    } else {
        return overrideValue;
    }
}

// Template specialization for float type
template<>
DataRefImport<float>::operator float()
{
    if (handle) {
        return XPLMGetDataf(handle);
    } else {
        return overrideValue;
    }
}

// Template specialization for double type
template<>
DataRefImport<double>::operator double()
{
    if (handle) {
        return XPLMGetDatad(handle);
    } else {
        return overrideValue;
    }
}

// Explicit template instantiations
template class DataRefImport<int>;
template class DataRefImport<bool>;
template class DataRefImport<float>;
template class DataRefImport<double>;

} // namespace XPlaneUtilities
