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

class SerializeHelper {

  public:
    /**
     * Serialize a string where the length is limited to 2^8 chars.
     * The length is stored in front of the string.
     */
    static void serializeString8(std::ostream& o, const std::string& s) {
#ifdef DEBUG
        if(s.size() > std::numeric_limits<uint8_t>::max()) {
            std::cout << "Error (serialize_::SerializeHelper::serializeString32): Not a short string!" << std::endl;
            exit(1);
        }
#endif
        serializeByte(o,s.size());
        if(s.size() > 0) o.write(s.data(),s.size());
    }

    static std::string deserializeString8(std::istream& i) {
        uint8_t size = deserializeByte(i);
        if(size > 0)
        {
            std::string result(size,'a');
            //char* data = new char[size];
            // const_cast necessary to avoid overhead by having to copy the string
            i.read(const_cast<char*>(result.data()),size);
            //delete[] data;
            return(result);
        }
        else return(std::string());
    }

    /**
     * Serialize a string where the length is limited to 2^16 chars.
     * The length is stored in front of the size.
     */
    static void serializeString16(std::ostream& o, const std::string& s) {
#ifdef DEBUG
        if(s.size() > std::numeric_limits<uint16_t>::max()) {
            std::cout << "Error (serialize_::SerializeHelper::serializeString16): Not a short string!" << std::endl;
            exit(1);
        }
#endif
        serializeShort(o,s.size());
        if(s.size() > 0) o.write(s.data(),s.size());
    }

    static std::string deserializeString16(std::istream& i) {
        uint16_t size = deserializeShort(i);
        if(size > 0)
        {
            std::string result(size,'a');
            //char* data = new char[size];
            // const_cast necessary to avoid overhead by having to copy the string
            i.read(const_cast<char*>(result.data()),size);
            //delete[] data;
            return(result);
        }
        else return(std::string());
    }

    /**
     * Serialize a string where the length is limited to 2^32 chars.
     * The length is stored in front of the size.
     */
    static void serializeString32(std::ostream& o, const std::string& s) {
#ifdef DEBUG
        if(s.size() > std::numeric_limits<uint32_t>::max())
        {
            std::cout << "Error (serialize_::SerializeHelper::serializeString32): Not a short string!" << std::endl;
            exit(1);
        }
#endif
        serializeInt(o,s.size());
        if(s.size() > 0) o.write(s.data(),s.size());
    }

    static std::string deserializeString32(std::istream& i) {
        uint32_t size = deserializeInt(i);
        if(size > 0)
        {
            std::string result(size,'a');
            //char* data = new char[size];
            // const_cast necessary to avoid overhead by having to copy the string
            i.read(const_cast<char*>(result.data()),size);
            //delete[] data;
            return(result);
        }
        else return(std::string());
    }

    /**
     * Serialize a string where the length is limited to 2^64 chars.
     * The length is stored in front of the size.
     */
    static void serializeString64(std::ostream& o, const std::string& s) {
        serializeLong(o,s.size());
        if(s.size() > 0) o.write(s.data(),s.size());
    }

    static std::string deserializeString64(std::istream& i) {
        uint64_t size = deserializeLong(i);
        if(size > 0)
        {
            std::string result(size,'a');
            //char* data = new char[size];
            // const_cast necessary to avoid overhead by having to copy the string
            i.read(const_cast<char*>(result.data()),size);
            //delete[] data;
            return(result);
        }
        else return(std::string());
    }

    /**
     * Serialize a string where the length is limited to maximall 2^64 chars.
     * The size of the number representing the length is variable.
     */
    static void serializeString(std::ostream& o, const std::string& s)
    {
        /*if(s.size() <= std::numeric_limits<uint8_t>::max())
        {
            serializeByte(o,0);
            serializeString8(o,s);
        }
        else if((s.size() > std::numeric_limits<uint8_t>::max()) && (s.size() <= std::numeric_limits<uint16_t>::max()))
        {
            serializeByte(o,1);
            serializeString16(o,s);
        }
        else if((s.size() > std::numeric_limits<uint16_t>::max()) && (s.size() <= std::numeric_limits<uint32_t>::max()))
        {
            serializeByte(o,2);
            serializeString32(o,s);
        }
        else
        {
            serializeByte(o,3);
            serializeString64(o,s);
        }*/
        serializeIntVar(o,s.size());
        if(s.size() > 0) o.write(s.data(),s.size());
    }

    static std::string deserializeString(std::istream& i) {
        uint64_t size = deserializeIntVar<uint64_t>(i);
        if(size > 0)
        {
            std::string result(size,'a');
            i.read(const_cast<char*>(result.data()),size);
            return(result);
        }
        else return(std::string());
    }

    /**
     * Serialize a single byte.
     */
    static void serializeByte(std::ostream& o, uint8_t b) {
        o.write(reinterpret_cast<const char*>(&b),sizeof(uint8_t));
    }

    static uint8_t deserializeByte(std::istream& i) {
        uint8_t data = 0;
        i.read(reinterpret_cast<char*>(&data),sizeof(uint8_t));
        return(data);
    }

    /**
     * Serialize a 2 byte integer.
     * htons is used.
     */
    static void serializeShort(std::ostream& o, uint16_t s) {
        uint16_t data = htons(s);
        o.write(reinterpret_cast<const char*>(&data),sizeof(uint16_t));
    }

    static uint16_t deserializeShort(std::istream& i) {
        uint16_t data = 0;
        i.read(reinterpret_cast<char*>(&data),sizeof(uint16_t));
        return(ntohs(data));
    }

    /**
     * Serialize a 4 byte integer.
     * htonl is used.
     */
    static void serializeInt(std::ostream& o, uint32_t i) {
        uint32_t data = htonl(i);
        o.write(reinterpret_cast<const char*>(&data),sizeof(uint32_t));
    }

    static uint32_t deserializeInt(std::istream& i) {
        uint32_t data = 0;
        i.read(reinterpret_cast<char*>(&data),sizeof(uint32_t));
        return(ntohl(data));
    }

    /**
     * Serialize an 8 byte integer.
     * The number is split in two parts and then serialized as two 4 byte integers.
     */
    static void serializeLong(std::ostream& o, uint64_t i) {
        /*uint8_t data[sizeof(uint64_t)];
        uint64_t curPow = std::pow(256,sizeof(uint64_t)-1);
        for(uint8_t x=sizeof(uint64_t)-1; x>0; --x)
        {
            data[x] = i/curPow;
            i = i%curPow;
            curPow /= 256;
        }
        data[0] = i;
        o.write(reinterpret_cast<const char*>(&data),sizeof(uint64_t));*/
        serializeInt(o,i/4294967296);
        serializeInt(o,i%4294967296);
    }

    static uint64_t deserializeLong(std::istream& i) {
        uint64_t num = 0;
        *reinterpret_cast<uint32_t*>(&num) = deserializeInt(i);
        *reinterpret_cast<uint32_t*>((&num)+1) = deserializeInt(i);
        num = (*reinterpret_cast<uint32_t*>(&num))*4294967296+(*reinterpret_cast<uint32_t*>((&num)+1));
        /*uint64_t curPow = 1;
        uint8_t data[sizeof(uint64_t)];
        i.read(reinterpret_cast<char*>(&data),sizeof(uint64_t));
        for(uint8_t x=0; x<sizeof(uint64_t); ++x)
        {
            num += data[x]*curPow;
            curPow *= 256;
        }*/
        return(num);
    }

    /**
     * Serialize an arbitrary integer of maximum size of 8 byte.
     * Depending on how large the number is the correct representation is chosen.
     * For this purpose an extra byte is stored in front of the number which tells of what type the representation is.
     */
    template<typename T>
    static void serializeIntVar(std::ostream& o, T i) {
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
    static T deserializeIntVar(std::istream& i) {
        uint8_t type = deserializeByte(i);
        if(type == 0) return(deserializeByte(i));
        else if(type == 1) return(deserializeShort(i));
        else if(type == 2) return(deserializeInt(i));
        else return(deserializeLong(i));
    }

    /**
     * Serialize a floating point number of size 4 byte.
     * The number is decomposed numerically and stored in a uint32_t variable in a special format.
     * It is then serialized as a uint32_t to get rid of byte order issues.
     */
    static void serializeFloat(std::ostream& os, float val) {
        uint32_t data = 0;
        int exp = 0;
        float mant = std::abs(std::frexp(val,&exp));
        uint32_t curPow = std::pow(2,7);
        float curPow_d = 1./2.;
        exp += 127;

        // Write sign
        if(std::signbit(val)) data = 1;
        data = data << 1;
        // Handle inf case
        if(mant == std::numeric_limits<double>::infinity())
        {
            mant = 0.;
            exp = 255;
        }
        // Handle quiet NaN case
        else if(isnan(val))
        {
            mant = 0.5;
            exp = 255;
        }

        // Set the bits in the number
        // Write exponent
        for(uint8_t i=0; i<8; ++i)
        {
            //mask = exp/curPow;
            data |= uint32_t(exp/curPow);
            exp = exp%curPow;
            curPow /= 2;
            data = data << 1;
        }
        // Write mantissa
        for(uint8_t i=0; i<22; ++i)
        {
            data |= uint32_t(mant/curPow_d);
            mant = fmod(mant,curPow_d);
            curPow_d /= 2.;
            data = data << 1;
        }
        data |= uint32_t(mant/curPow_d);

        serializeInt(os,data);
        //os.write(reinterpret_cast<const char*>(&data),sizeof(uint32_t));
    }

    static float deserializeFloat(std::istream& is) {
        uint32_t data = deserializeInt(is);
        float val = 0;
        int exp = 0;
        uint32_t curPow = 1;
        float curPow_d = 1./std::pow(2.,23);

        // Read data from stream
        //is.read(reinterpret_cast<char*>(&data),sizeof(uint32_t));

        // Deserialize mantissa
        for(uint8_t i=0; i<23; ++i)
        {
            val += (data & uint32_t(1))*curPow_d;
            curPow_d *= 2.;
            data = data >> 1;
        }
        // Deserialize exponent
        for(uint8_t i=0; i<8; ++i)
        {
            exp += (data & uint32_t(1))*curPow;
            curPow *= 2;
            data = data >> 1;
        }
        // Handle inf case
        if(val == 0 && exp == 255)
        {
            if((data & uint64_t(1)) == 1) val = -std::numeric_limits<double>::infinity();
            else val = std::numeric_limits<double>::infinity();
            return val;
        }
        // Handle quiet NaN case
        else if(val == 0.5 && exp == 255)
        {
            if((data & uint64_t(1)) == 1) val = -std::numeric_limits<double>::quiet_NaN();
            else val = std::numeric_limits<double>::quiet_NaN();
            return val;
        }
        // Normal number
        if((data & uint64_t(1)) == 1) val = -val;
        exp -= 127;
        // Multiply them together
        val = std::ldexp(val,exp);
        return(val);
    }

    /**
     * Serialize a floating point number of size 8 byte.
     * The number is decomposed numerically and stored in a uint64_t variable in a special format.
     * It is then serialized as a uint64_t to get rid of byte order issues.
     */
    static void serializeDouble(std::ostream& os, double val) {
        uint64_t data = 0;
        int exp = 0;
        double mant = std::abs(std::frexp(val,&exp));
        uint64_t curPow = std::pow(2,10);
        double curPow_d = 1./2.;
        exp += 1023;

        // Write sign
        if(std::signbit(val)) data = 1;
        data = data << 1;
        // Handle inf case
        if(mant == std::numeric_limits<double>::infinity())
        {
            mant = 0.;
            exp = 2047;
        }
        // Handle quiet NaN case
        else if(isnan(val))
        {
            mant = 0.5;
            exp = 2047;
        }

        // Set the bits in the number
        // Write exponent
        for(uint8_t i=0; i<11; ++i)
        {
            //mask = exp/curPow;
            data |= uint64_t(exp/curPow);
            exp = exp%curPow;
            curPow /= 2;
            data = data << 1;
        }
        // Write mantissa
        for(uint8_t i=0; i<51; ++i)
        {
            data |= uint64_t(mant/curPow_d);
            mant = fmod(mant,curPow_d);
            curPow_d /= 2.;
            data = data << 1;
        }
        data |= uint64_t(mant/curPow_d);

        serializeLong(os,data);
        //os.write(reinterpret_cast<const char*>(&data),sizeof(uint64_t));
    }

    static double deserializeDouble(std::istream& is) {
        uint64_t data = deserializeLong(is);
        double val = 0;
        int exp = 0;
        uint64_t curPow = 1;
        double curPow_d = 1./std::pow(2.,52);

        // Read data from stream
        //is.read(reinterpret_cast<char*>(&data),sizeof(uint64_t));

        // Deserialize mantissa
        for(uint8_t i=0; i<52; ++i)
        {
            val += (data & uint64_t(1))*curPow_d;
            curPow_d *= 2.;
            data = data >> 1;
        }
        // Deserialize exponent
        for(uint8_t i=0; i<11; ++i)
        {
            exp += (data & uint64_t(1))*curPow;
            curPow *= 2;
            data = data >> 1;
        }
        // Handle inf case
        if(val == 0 && exp == 2047)
        {
            if((data & uint64_t(1)) == 1) val = -std::numeric_limits<double>::infinity();
            else val = std::numeric_limits<double>::infinity();
            return val;
        }
        // Handle quiet NaN case
        else if(val == 0.5 && exp == 2047)
        {
            if((data & uint64_t(1)) == 1) val = -std::numeric_limits<double>::quiet_NaN();
            else val = std::numeric_limits<double>::quiet_NaN();
            return val;
        }
        // Normal number
        if((data & uint64_t(1)) == 1) val = -val;
        exp -= 1023;
        // Multiply them together
        val = std::ldexp(val,exp);
        return(val);
    }

    static void serializeByteArray(std::ostream& o, uint64_t len, const uint8_t* data) {
        serializeIntVar(o,len);
        for(size_t i=0; i<len; ++i) serializeByte(o,data[i]);
    //    o.write(reinterpret_cast<const char*>(&data),sizeof(uint8_t)*len);
    }

    static uint8_t* deserializeByteArray(std::istream& is, uint64_t& len) {
        len = deserializeIntVar<uint64_t>(is);
        uint8_t* data = new uint8_t[len];
        for(size_t i=0; i<len; ++i) data[i] = deserializeByte(is);
        //is.read(reinterpret_cast<char*>(&data),sizeof(uint8_t)*len);
        return(data);
    }

    static void serializeShortArray(std::ostream& o, uint64_t len, const uint16_t* data)
    {
        serializeIntVar(o,len);
        for(size_t i=0; i<len; ++i) serializeShort(o,data[i]);
    }

    static uint16_t* deserializeShortArray(std::istream& is, uint64_t& len)
    {
        len = deserializeIntVar<uint64_t>(is);
        uint16_t* data = new uint16_t[len];
        for(size_t i=0; i<len; ++i) data[i] = deserializeShort(is);
        return(data);
    }

    static void serializeIntArray(std::ostream& o, uint64_t len, const uint32_t* data)
    {
        serializeIntVar(o,len);
        for(size_t i=0; i<len; ++i) serializeInt(o,data[i]);
    }

    static uint32_t* deserializeIntArray(std::istream& is, uint64_t& len)
    {
        len = deserializeIntVar<uint64_t>(is);
        uint32_t* data = new uint32_t[len];
        for(size_t i=0; i<len; ++i) data[i] = deserializeInt(is);
        return(data);
    }

    static void serializeLongArray(std::ostream& o, uint64_t len, const uint64_t* data)
    {
        serializeIntVar(o,len);
        for(size_t i=0; i<len; ++i) serializeLong(o,data[i]);
    }

    static uint64_t* deserializeLongArray(std::istream& is, uint64_t& len)
    {
        len = deserializeIntVar<uint64_t>(is);
        uint64_t* data = new uint64_t[len];
        for(size_t i=0; i<len; ++i) data[i] = deserializeLong(is);
        return(data);
    }

    static void serializeFloatArray(std::ostream& o, uint64_t len, const float* data)
    {
        serializeIntVar(o,len);
        for(size_t i=0; i<len; ++i) serializeFloat(o,data[i]);
    }

    static float* deserializeFloatArray(std::istream& is, uint64_t& len)
    {
        len = deserializeIntVar<uint64_t>(is);
        float* data = new float[len];
        for(size_t i=0; i<len; ++i) data[i] = deserializeFloat(is);
        return(data);
    }

    static void serializeDoubleArray(std::ostream& o, uint64_t len, const double* data)
    {
        serializeIntVar(o,len);
        for(size_t i=0; i<len; ++i) serializeDouble(o,data[i]);
    }

    static double* deserializeDoubleArray(std::istream& is, uint64_t& len)
    {
        len = deserializeIntVar<uint64_t>(is);
        double* data = new double[len];
        for(size_t i=0; i<len; ++i) data[i] = deserializeDouble(is);
        return(data);
    }

    /**
     * Convenience methods that do serialize correctly for the passed type.
     * For strings it takes the variable sized variant.
     */
    template<typename T> static void serialize(std::ostream& os, const T& val);
    template<typename T> static T deserialize(std::istream& is);
    template<typename T> static void serializeArray(std::ostream& os, const uint64_t& len, const T* val);
    template<typename T> static T* deserializeArray(std::istream& is, uint64_t& len);
};

template<typename T>
void SerializeHelper::serialize(std::ostream& os, const T& val)
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
