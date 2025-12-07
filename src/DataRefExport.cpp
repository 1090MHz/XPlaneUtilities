#include <XPlaneUtilities/DataRefExport.h>

namespace XPlaneUtilities
{

// Template specialization for int type - read-write
template <>
DataRefExport<int>::DataRefExport(const std::string& name, void* ref,
                                  std::function<int(void*)> onRd,
                                  std::function<void(void*, int)> onWr)
    : ownerRef(ref), onRead(onRd), onWrite(onWr)
{
    xpDataRef = XPLMRegisterDataAccessor(
        name.c_str(), xplmType_Int,
        1, // writable
        // Int read callback
        [](void* r)
        {
            auto self = reinterpret_cast<DataRefExport<int>*>(r);
            return self->onRead(self->ownerRef);
        },
        // Int write callback
        [](void* r, int v)
        {
            auto self = reinterpret_cast<DataRefExport<int>*>(r);
            self->onWrite(self->ownerRef, v);
        },
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        this, this);
}

// Template specialization for int type - read-only
template <>
DataRefExport<int>::DataRefExport(const std::string& name, void* ref,
                                  std::function<int(void*)> onRd)
    : ownerRef(ref), onRead(onRd), onWrite(nullptr)
{
    xpDataRef = XPLMRegisterDataAccessor(
        name.c_str(), xplmType_Int,
        0, // not writable
        // Int read callback
        [](void* r)
        {
            auto self = reinterpret_cast<DataRefExport<int>*>(r);
            return self->onRead(self->ownerRef);
        },
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, this, nullptr);
}

// Template specialization for float type - read-write
template <>
DataRefExport<float>::DataRefExport(const std::string& name, void* ref,
                                    std::function<float(void*)> onRd,
                                    std::function<void(void*, float)> onWr)
    : ownerRef(ref), onRead(onRd), onWrite(onWr)
{
    xpDataRef = XPLMRegisterDataAccessor(
        name.c_str(), xplmType_Float,
        1, // writable
        nullptr, nullptr,
        // Float read callback
        [](void* r)
        {
            auto self = reinterpret_cast<DataRefExport<float>*>(r);
            return self->onRead(self->ownerRef);
        },
        // Float write callback
        [](void* r, float v)
        {
            auto self = reinterpret_cast<DataRefExport<float>*>(r);
            self->onWrite(self->ownerRef, v);
        },
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, this, this);
}

// Template specialization for float type - read-only
template <>
DataRefExport<float>::DataRefExport(const std::string& name, void* ref,
                                    std::function<float(void*)> onRd)
    : ownerRef(ref), onRead(onRd), onWrite(nullptr)
{
    xpDataRef = XPLMRegisterDataAccessor(
        name.c_str(), xplmType_Float,
        0, // not writable
        nullptr, nullptr,
        // Float read callback
        [](void* r)
        {
            auto self = reinterpret_cast<DataRefExport<float>*>(r);
            return self->onRead(self->ownerRef);
        },
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, this,
        nullptr);
}

// Destructor - unregister the dataref
template <typename T> DataRefExport<T>::~DataRefExport()
{
    if (xpDataRef)
    {
        XPLMUnregisterDataAccessor(xpDataRef);
        xpDataRef = nullptr;
    }
}

// Explicit template instantiations
template class DataRefExport<int>;
template class DataRefExport<float>;

} // namespace XPlaneUtilities
