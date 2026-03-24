#include <XPlaneUtilities/DataRefAccess.h>
#include <XPlaneUtilities/XPlaneLog.h>

namespace XPlaneUtilities
{
namespace
{
template <typename T>
bool supportsType(XPLMDataTypeID supportedTypes)
{
    (void)supportedTypes;
    return false;
}

template <>
bool supportsType<int>(XPLMDataTypeID supportedTypes)
{
    return (supportedTypes & xplmType_Int) != 0;
}

template <>
bool supportsType<bool>(XPLMDataTypeID supportedTypes)
{
    return (supportedTypes & xplmType_Int) != 0;
}

template <>
bool supportsType<float>(XPLMDataTypeID supportedTypes)
{
    return (supportedTypes & xplmType_Float) != 0;
}

template <>
bool supportsType<double>(XPLMDataTypeID supportedTypes)
{
    return (supportedTypes & xplmType_Double) != 0;
}

template <typename T>
void logTypeMismatch(const std::string& dataRefName)
{
    (void)dataRefName;
}

template <>
void logTypeMismatch<int>(const std::string& dataRefName)
{
    XPlaneLog::warn("DataRef '" + dataRefName + "' does not expose int access");
}

template <>
void logTypeMismatch<bool>(const std::string& dataRefName)
{
    XPlaneLog::warn("DataRef '" + dataRefName + "' does not expose bool/int access");
}

template <>
void logTypeMismatch<float>(const std::string& dataRefName)
{
    XPlaneLog::warn("DataRef '" + dataRefName + "' does not expose float access");
}

template <>
void logTypeMismatch<double>(const std::string& dataRefName)
{
    XPlaneLog::warn("DataRef '" + dataRefName + "' does not expose double access");
}
} // namespace

template <typename T>
DataRefAccess<T>::DataRefAccess(const std::string& name)
    : dataRefName(name)
{
    handle = XPLMFindDataRef(name.c_str());
    if (!handle)
    {
        throw std::runtime_error("Invalid DataRef: " + name);
    }

    supportedTypes = XPLMGetDataRefTypes(handle);
    writable = XPLMCanWriteDataRef(handle) != 0;
    compatibleType = supportsType<T>(supportedTypes);

    if (!compatibleType)
    {
        logTypeMismatch<T>(dataRefName);
    }
}

template <typename T>
DataRefAccess<T>::DataRefAccess(const std::string& name, T defaultValue)
    : dataRefName(name), overrideValue(defaultValue)
{
    handle = XPLMFindDataRef(name.c_str());
    if (!handle)
    {
        XPlaneLog::warn("DataRef '" + name + "' not available, using default value");
        return;
    }

    supportedTypes = XPLMGetDataRefTypes(handle);
    writable = XPLMCanWriteDataRef(handle) != 0;
    compatibleType = supportsType<T>(supportedTypes);

    if (!compatibleType)
    {
        logTypeMismatch<T>(dataRefName);
    }
}

template <typename T>
T DataRefAccess<T>::get() const
{
    return static_cast<T>(*this);
}

template <typename T>
DataRefAccess<T>& DataRefAccess<T>::operator=(T value)
{
    set(value);
    return *this;
}

template <>
DataRefAccess<int>::operator int() const
{
    if (!handle || !compatibleType)
    {
        return overrideValue;
    }

    return XPLMGetDatai(handle);
}

template <>
DataRefAccess<bool>::operator bool() const
{
    if (!handle || !compatibleType)
    {
        return overrideValue;
    }

    return XPLMGetDatai(handle) != 0;
}

template <>
DataRefAccess<float>::operator float() const
{
    if (!handle || !compatibleType)
    {
        return overrideValue;
    }

    return XPLMGetDataf(handle);
}

template <>
DataRefAccess<double>::operator double() const
{
    if (!handle || !compatibleType)
    {
        return overrideValue;
    }

    return XPLMGetDatad(handle);
}

template <>
bool DataRefAccess<int>::set(int value)
{
    if (!handle || !writable || !compatibleType)
    {
        return false;
    }

    XPLMSetDatai(handle, value);
    return true;
}

template <>
bool DataRefAccess<bool>::set(bool value)
{
    if (!handle || !writable || !compatibleType)
    {
        return false;
    }

    XPLMSetDatai(handle, value ? 1 : 0);
    return true;
}

template <>
bool DataRefAccess<float>::set(float value)
{
    if (!handle || !writable || !compatibleType)
    {
        return false;
    }

    XPLMSetDataf(handle, value);
    return true;
}

template <>
bool DataRefAccess<double>::set(double value)
{
    if (!handle || !writable || !compatibleType)
    {
        return false;
    }

    XPLMSetDatad(handle, value);
    return true;
}

template class DataRefAccess<int>;
template class DataRefAccess<bool>;
template class DataRefAccess<float>;
template class DataRefAccess<double>;

} // namespace XPlaneUtilities