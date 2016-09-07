#ifndef SERIALIZE_SERIALIZEHELPER_H
#define SERIALIZE_SERIALIZEHELPER_H

#include <iostream>
#include <cstdint>
#include <netinet/in.h>
#include <limits>
#include <bitset>
#include <cmath>
#include <type_traits>

namespace BTC {

class SerializeHelper
{
public:
    /**
     * Serialize a string where the length is limited to 2^8 chars.
     * The length is stored in front of the string.
     */
    static void serializeString8(std::ostream& o, const std::string& s);
    static std::string deserializeString8(std::istream& i);

    /**
     * Serialize a string where the length is limited to 2^16 chars.
     * The length is stored in front of the size.
     */
    static void serializeString16(std::ostream& o, const std::string& s);
    static std::string deserializeString16(std::istream& i);

    /**
     * Serialize a string where the length is limited to 2^32 chars.
     * The length is stored in front of the size.
     */
    static void serializeString32(std::ostream& o, const std::string& s);
    static std::string deserializeString32(std::istream& i);

    /**
     * Serialize a string where the length is limited to 2^64 chars.
     * The length is stored in front of the size.
     */
    static void serializeString64(std::ostream& o, const std::string& s);
    static std::string deserializeString64(std::istream& i);

    /**
     * Serialize a string where the length is limited to maximall 2^64 chars.
     * The size of the number representing the length is variable.
     */
    static void serializeString(std::ostream& o, const std::string& s);
    static std::string deserializeString(std::istream& i);

    /**
     * Serialize a single byte.
     */
    static void serializeByte(std::ostream& o, uint8_t i);
    static uint8_t deserializeByte(std::istream& i);

    /**
     * Serialize a 2 byte integer.
     * htons is used.
     */
    static void serializeShort(std::ostream& o, uint16_t i);
    static uint16_t deserializeShort(std::istream& i);

    /**
     * Serialize a 4 byte integer.
     * htonl is used.
     */
    static void serializeInt(std::ostream& o, uint32_t i);
    static uint32_t deserializeInt(std::istream& i);

    /**
     * Serialize an 8 byte integer.
     * The number is split in two parts and then serialized as two 4 byte integers.
     */
    static void serializeLong(std::ostream& o, uint64_t i);
    static uint64_t deserializeLong(std::istream& i);

    /**
     * Serialize an arbitrary integer of maximum size of 8 byte.
     * Depending on how large the number is the correct representation is chosen.
     * For this purpose an extra byte is stored in front of the number which tells of what type the representation is.
     */
    template<typename T> static void serializeIntVar(std::ostream& o, T i);
    template<typename T> static T deserializeIntVar(std::istream& i);

    /**
     * Serialize a floating point number of size 4 byte.
     * The number is decomposed numerically and stored in a uint32_t variable in a special format.
     * It is then serialized as a uint32_t to get rid of byte order issues.
     */
    static void serializeFloat(std::ostream& o, float i);
    static float deserializeFloat(std::istream& i);

    /**
     * Serialize a floating point number of size 8 byte.
     * The number is decomposed numerically and stored in a uint64_t variable in a special format.
     * It is then serialized as a uint64_t to get rid of byte order issues.
     */
    static void serializeDouble(std::ostream& o, double i);
    static double deserializeDouble(std::istream& i);

    static void serializeByteArray(std::ostream& o, uint64_t len, const uint8_t* data);
    static uint8_t* deserializeByteArray(std::istream& i, uint64_t& len);

    static void serializeShortArray(std::ostream& o, uint64_t len, const uint16_t* data);
    static uint16_t* deserializeShortArray(std::istream& i, uint64_t& len);

    static void serializeIntArray(std::ostream& o, uint64_t len, const uint32_t* data);
    static uint32_t* deserializeIntArray(std::istream& i, uint64_t& len);

    static void serializeLongArray(std::ostream& o, uint64_t len, const uint64_t* data);
    static uint64_t* deserializeLongArray(std::istream& i, uint64_t& len);

    static void serializeFloatArray(std::ostream& o, uint64_t len, const float* data);
    static float* deserializeFloatArray(std::istream& i, uint64_t& len);

    static void serializeDoubleArray(std::ostream& o, uint64_t len, const double* data);
    static double* deserializeDoubleArray(std::istream& i, uint64_t& len);

    /**
     * Convenience methods that do serialize correctly for the passed type.
     * For strings it takes the variable sized variant.
     */
    //static void serialize(std::ostream& os, const std::string& val);
    template<typename T> static void serialize(std::ostream& os, const T& val);
    template<typename T> static T deserialize(std::istream& is);
    template<typename T> static void serializeArray(std::ostream& os, const uint64_t& len, const T* val);
    template<typename T> static T* deserializeArray(std::istream& is, uint64_t& len);
};

template<typename T>
void SerializeHelper::serializeIntVar(std::ostream& o, T i)
{
//    static_assert(std::is_integral<T>::value,"Error (serialize_::SerializeHelper::serializeIntVar): Not an integral type!");
//    static_assert(std::is_unsigned<T>::value,"Error (serialize_::SerializeHelper::serializeIntVar): Integral type not unsigned!");
    if(i <= std::numeric_limits<uint8_t>::max())
    {
        serializeByte(o,0);
        serializeByte(o,i);
    }
    else if((i > std::numeric_limits<uint8_t>::max()) && (i <= std::numeric_limits<uint16_t>::max()))
    {
        serializeByte(o,1);
        serializeShort(o,i);
    }
    else if((i > std::numeric_limits<uint16_t>::max()) && (i <= std::numeric_limits<uint32_t>::max()))
    {
        serializeByte(o,2);
        serializeInt(o,i);
    }
    else
    {
        serializeByte(o,3);
        serializeLong(o,i);
    }
}

template<typename T>
T SerializeHelper::deserializeIntVar(std::istream& i)
{
    uint8_t type = deserializeByte(i);
    if(type == 0) return(deserializeByte(i));
    else if(type == 1) return(deserializeShort(i));
    else if(type == 2) return(deserializeInt(i));
    else return(deserializeLong(i));
}

template<typename T>
static void serialize(std::ostream& os, const T& val)
{
    std::cout << "Error (serialize_::SerializeHelper::serialize): Method is not applicable for this type!" << std::endl;
    exit(1);
}

template<typename T>
static T deserialize(std::ostream& is)
{
    std::cout << "Error (serialize_::SerializeHelper::deserialize): Method is not applicable for this type!" << std::endl;
    exit(1);
}

template<typename T>
static void serializeArray(std::ostream& os, const uint64_t& len, const T* val)
{
    std::cout << "Error (serialize_::SerializeHelper::serialize): Method is not applicable for this type!" << std::endl;
    exit(1);
}

template<typename T>
static T* deserializeArray(std::ostream& is, uint64_t& len)
{
    std::cout << "Error (serialize_::SerializeHelper::deserialize): Method is not applicable for this type!" << std::endl;
    exit(1);
}

}

#endif
