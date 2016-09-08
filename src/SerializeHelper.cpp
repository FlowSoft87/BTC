#include "SerializeHelper.h"

namespace BTC {

template<>
void SerializeHelper::serialize<std::string>(std::ostream& os, const std::string& i)
{
    serializeString(os,i);
}

template<>
void SerializeHelper::serialize<uint8_t>(std::ostream& os, const uint8_t& i)
{
    serializeByte(os,i);
}

template<>
void SerializeHelper::serialize<uint16_t>(std::ostream& os, const uint16_t& i)
{
    serializeShort(os,i);
}

template<>
void SerializeHelper::serialize<uint32_t>(std::ostream& os, const uint32_t& i)
{
    serializeInt(os,i);
}

template<>
void SerializeHelper::serialize<uint64_t>(std::ostream& os, const uint64_t& i)
{
    serializeLong(os,i);
}

template<>
void SerializeHelper::serialize<float>(std::ostream& os, const float& i)
{
    serializeFloat(os,i);
}

template<>
void SerializeHelper::serialize<double>(std::ostream& os, const double& i)
{
    serializeDouble(os,i);
}

template<>
std::string SerializeHelper::deserialize<std::string>(std::istream& i)
{
    return(deserializeString(i));
}

template<>
uint8_t SerializeHelper::deserialize<uint8_t>(std::istream& i)
{
    return(deserializeByte(i));
}

template<>
uint16_t SerializeHelper::deserialize<uint16_t>(std::istream& i)
{
    return(deserializeShort(i));
}

template<>
uint32_t SerializeHelper::deserialize<uint32_t>(std::istream& i)
{
    return(deserializeInt(i));
}

template<>
uint64_t SerializeHelper::deserialize<uint64_t>(std::istream& i)
{
    return(deserializeLong(i));
}

template<>
float SerializeHelper::deserialize<float>(std::istream& i)
{
    return(deserializeFloat(i));
}

template<>
double SerializeHelper::deserialize<double>(std::istream& i)
{
    return(deserializeDouble(i));
}

template<>
void SerializeHelper::serializeArray<uint8_t>(std::ostream& os, const uint64_t& len, const uint8_t* i)
{
    serializeByteArray(os,len,i);
}

template<>
void SerializeHelper::serializeArray<uint16_t>(std::ostream& os, const uint64_t& len, const uint16_t* i)
{
    serializeShortArray(os,len,i);
}

template<>
void SerializeHelper::serializeArray<uint32_t>(std::ostream& os, const uint64_t& len, const uint32_t* i)
{
    serializeIntArray(os,len,i);
}

template<>
void SerializeHelper::serializeArray<uint64_t>(std::ostream& os, const uint64_t& len, const uint64_t* i)
{
    serializeLongArray(os,len,i);
}

template<>
void SerializeHelper::serializeArray<float>(std::ostream& os, const uint64_t& len, const float* i)
{
    serializeFloatArray(os,len,i);
}

template<>
void SerializeHelper::serializeArray<double>(std::ostream& os, const uint64_t& len, const double* i)
{
    serializeDoubleArray(os,len,i);
}

template<>
uint8_t* SerializeHelper::deserializeArray<uint8_t>(std::istream& i, uint64_t& len)
{
    return(deserializeByteArray(i,len));
}

template<>
uint16_t* SerializeHelper::deserializeArray<uint16_t>(std::istream& i, uint64_t& len)
{
    return(deserializeShortArray(i,len));
}

template<>
uint32_t* SerializeHelper::deserializeArray<uint32_t>(std::istream& i, uint64_t& len)
{
    return(deserializeIntArray(i,len));
}

template<>
uint64_t* SerializeHelper::deserializeArray<uint64_t>(std::istream& i, uint64_t& len)
{
    return(deserializeLongArray(i,len));
}

template<>
float* SerializeHelper::deserializeArray<float>(std::istream& i, uint64_t& len)
{
    return(deserializeFloatArray(i,len));
}

template<>
double* SerializeHelper::deserializeArray<double>(std::istream& i, uint64_t& len)
{
    return(deserializeDoubleArray(i,len));
}

}

