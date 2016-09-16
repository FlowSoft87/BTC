#ifndef SERIALIZE_SERIALIZEHELPER_H
#define SERIALIZE_SERIALIZEHELPER_H

#include <iostream>
#include <netinet/in.h>
#include <limits>
#include <cmath>

namespace BTC {
namespace serialize_ {

/**
 * Serialize a single byte.
 */
void serializeByte(std::ostream& o, UINT8_T b) {
    o.write(reinterpret_cast<const char*>(&b),1);
}

UINT8_T deserializeByte(std::istream& i) {
    UINT8_T data = 0;
    i.read(reinterpret_cast<char*>(&data),1);
    return(data);
}

/**
 * Serialize a 2 byte integer.
 */
template<typename UINT16_T>
void serializeShort(std::ostream& o, UINT16_T s) {
    serializeByte(o,s/256);
    serializeByte(o,s%256);
}

UINT16_T deserializeShort(std::istream& i) {
    UINT16_T data = 0;
    data += ((UINT16_T)deserializeByte(i))*256;
    data += deserializeByte(i);
    return(data);
}

/**
 * Serialize a 4 byte integer.
 */
template<typename UINT32_T>
void serializeInt(std::ostream& o, const UINT32_T& i) {
    UINT16_T data = i/65536;
    serializeShort(o,data);
    data = i%65536;
    serializeShort(o,data);
}

UINT32_T deserializeInt(std::istream& i) {
    UINT32_T data = 0;
    data += ((UINT32_T)deserializeShort(i))*65536;
    data += deserializeShort(i);
    return(data);
}

/**
 * Serialize an 8 byte integer.
 * The number is split in two parts and then serialized as two 4 byte integers.
 */
template<typename UINT64_T>
void serializeLong(std::ostream& o, const UINT64_T& i) {
    UINT32_T data = i/4294967296;
    serializeInt(o,data);
    data = i%4294967296;
    serializeInt(o,data);
}

UINT64_T deserializeLong(std::istream& i) {
    UINT64_T data = 0;
    data += ((UINT64_T)deserializeInt(i))*4294967296;
    data += deserializeInt(i);
    return(data);
}

/**
 * Serialize an arbitrary integer of maximum size of 8 byte.
 * Depending on how large the number is the correct representation is chosen.
 * For this purpose an extra byte is stored in front of the number which tells of what type the representation is.
 */
template<typename T>
void serializeIntVar(std::ostream& o, const T& i) {
    if(i <= 256) {
        serializeByte(o,0);
        serializeByte(o,i);
    } else if((i > 256) && (i <= 65536)) {
        serializeByte(o,1);
        serializeShort(o,i);
    } else if((i > 65536) && (i <= 4294967296)) {
        serializeByte(o,2);
        serializeInt(o,i);
    } else {
        serializeByte(o,3);
        serializeLong(o,i);
    }
}

template<typename T>
T deserializeIntVar(std::istream& i) {
    UINT8_T type = deserializeByte(i);
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
void serializeFloat(std::ostream& os, const FLOAT_T& val) {
    UINT32_T data = 0;
    int exp = 0;
    FLOAT_T mant = std::frexp(val,&exp);
    if (mant < 0.f) {
        mant = -mant;
    }
    UINT32_T curPow = std::pow(2,7);
    FLOAT_T curPow_d = 1.f/4.f;
    exp += 127;
    // Write sign
    if(std::signbit(val)) data = 1;
    data = data << 1;
    if(mant == std::numeric_limits<FLOAT_T>::infinity()) {
        // Handle inf case
        mant = 0.f;
        exp = 255;
    } else if(isnan(val)) {
        // Handle quiet NaN case
        mant = 0.5f;
        exp = 255;
    }
    // Set the bits in the number
    // Write exponent
    for(UINT8_T i=0; i<8; ++i) {
        //mask = exp/curPow;
        data |= (UINT32_T)(exp/curPow);
        exp = exp%curPow;
        curPow /= 2;
        data = data << 1;
    }
    // Write mantissa
    mant = std::fmod(mant,1.f/2.f);
    for(UINT8_T i=0; i<22; ++i) {
        data |= (UINT32_T)(mant/curPow_d);
        mant = fmod(mant,curPow_d);
        curPow_d /= 2.f;
        data = data << 1;
    }
    data |= (UINT32_T)(mant/curPow_d);
    serializeInt(os,data);
}

/**
 * Deserialize a floating point number of size 4 byte.
 */
FLOAT_T deserializeFloat(std::istream& is) {
    UINT32_T data = deserializeInt(is);
    FLOAT_T val = 0.5f;
    int exp = 0;
    UINT32_T curPow = 1;
    FLOAT_T curPow_d = 1.f/std::pow(2.f,24);
    // Deserialize mantissa
    for(UINT8_T i=0; i<23; ++i) {
        val += (data & (UINT32_T)(1))*curPow_d;
        curPow_d *= 2.f;
        data = data >> 1;
    }
    // Deserialize exponent
    for(UINT8_T i=0; i<8; ++i) {
        exp += (data & (UINT32_T)(1))*curPow;
        curPow *= 2;
        data = data >> 1;
    }
    if(val == 0 && exp == 255) {
        // Handle inf case
        if((data & (UINT32_T)(1)) == 1) val = -std::numeric_limits<FLOAT_T>::infinity();
        else val = std::numeric_limits<FLOAT_T>::infinity();
        return val;
    } else if(val == 0.5f && exp == 255) {
        // Handle quiet NaN case
        if((data & (UINT32_T)(1)) == 1) val = -std::numeric_limits<FLOAT_T>::quiet_NaN();
        else val = std::numeric_limits<FLOAT_T>::quiet_NaN();
        return val;
    }
    // Normal number
    if((data & (UINT32_T)(1)) == 1) val = -val;
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
void serializeDouble(std::ostream& os, const DOUBLE_T& val) {
    UINT64_T data = 0;
    int exp = 0;
    DOUBLE_T mant = std::frexp(val,&exp);
    if (mant < 0.d) {
        mant = -mant;
    }
    UINT64_T curPow = std::pow(2,10);
    DOUBLE_T curPow_d = 1.d/4.d;
    exp += 1023;
    // Write sign
    if(std::signbit(val)) data = 1;
    data = data << 1;
    if(mant == std::numeric_limits<DOUBLE_T>::infinity()) {
        // Handle inf case
        mant = 0.d;
        exp = 2047;
    } else if(isnan(val)) {
        // Handle quiet NaN case
        mant = 0.5d;
        exp = 2047;
    }
    // Set the bits in the number
    // Write exponent
    for(UINT8_T i=0; i<11; ++i) {
        //mask = exp/curPow;
        data |= (UINT64_T)(exp/curPow);
        exp = exp%curPow;
        curPow /= 2;
        data = data << 1;
    }
    // Write mantissa
    mant = std::fmod(mant,1.d/2.d);
    for(UINT8_T i=0; i<51; ++i){
        data |= (UINT64_T)(mant/curPow_d);
        mant = fmod(mant,curPow_d);
        curPow_d /= 2.d;
        data = data << 1;
    }
    data |= (UINT64_T)(mant/curPow_d);
    serializeLong(os,data);
}

DOUBLE_T deserializeDouble(std::istream& is) {
    UINT64_T data = deserializeLong(is);
    DOUBLE_T val = 0.5d;
    int exp = 0;
    UINT64_T curPow = 1;
    DOUBLE_T curPow_d = 1.d/std::pow(2.d,53);
    // Deserialize mantissa
    for(UINT8_T i=0; i<52; ++i) {
        val += (data & (UINT64_T)(1))*curPow_d;
        curPow_d *= 2.d;
        data = data >> 1;
    }
    // Deserialize exponent
    for(UINT8_T i=0; i<11; ++i) {
        exp += (data & (UINT64_T)(1))*curPow;
        curPow *= 2;
        data = data >> 1;
    }
    // Handle inf case
    if(val == 0.d && exp == 2047)
    {
        if((data & (UINT64_T)(1)) == 1) val = -std::numeric_limits<DOUBLE_T>::infinity();
        else val = std::numeric_limits<DOUBLE_T>::infinity();
        return val;
    }
    // Handle quiet NaN case
    else if(val == 0.5d && exp == 2047)
    {
        if((data & (UINT64_T)(1)) == 1) val = -std::numeric_limits<DOUBLE_T>::quiet_NaN();
        else val = std::numeric_limits<DOUBLE_T>::quiet_NaN();
        return val;
    }
    // Normal number
    if((data & (UINT64_T)(1)) == 1) val = -val;
    exp -= 1023;
    // Multiply them together
    val = std::ldexp(val,exp);
    return(val);
}

/*template<typename T>
void serializeFloat4(std::ostream& os, T value) {
    // Get sign
    bool s = (value < 0);
    if (s) {
        value = -value;
    }
    // Get exponent
    int e = (int)std::log2(value);
    // Get mantissa
    UINT32_T m = (value/std::pow(2.f,e)-1)*8388608;
    // Assemble final form
    UINT32_T num = 0;
    if (sign) {
        num = 1;
    }
    num = num << 8;
    e += 127;
    num |= e;
    num = num << 23;
    serializeInt(os,num);
}

FLOAT_T deserializeFloat4(std::istream& is) {
    UINT32_T value = deserializeInt(is);
    // Get mantissa
    UINT32_T m = value%8388608;
    value /= 8388608;
    // Get exponent
    int e = value%256-127;
    // Get sign
    value /= 256;
    // Build result
    FLOAT_T result = m*std::pow(2.f,e);
    if (value == 1) {
        result = -result;
    }
    return result;
}*/

/**
 * Serialize a string where the length is limited to 2^8 chars.
 * The length is stored in front of the string.
 */
void serializeString8(std::ostream& o, const STRING_T& s) {
#ifdef DEBUG
    if(s.size() > 256) {
        std::cout << "Error (serialize_::SerializeHelper::serializeString8): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeByte(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

STRING_T deserializeString8(std::istream& i) {
    UINT8_T size = deserializeByte(i);
    if(size > 0) {
        STRING_T result(size,'a');
        i.read(const_cast<char*>(result.data()),size);
        return(result);
    } else {
        return(STRING_T());
    }
}

/**
 * Serialize a string where the length is limited to 2^16 chars.
 * The length is stored in front of the size.
 */
void serializeString16(std::ostream& o, const STRING_T& s) {
#ifdef DEBUG
    if(s.size() > 65536) {
        std::cout << "Error (serialize_::SerializeHelper::serializeString16): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeShort(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

STRING_T deserializeString16(std::istream& i) {
    UINT16_T size = deserializeShort(i);
    if(size > 0) {
        STRING_T result(size,'a');
        // const_cast necessary to avoid overhead by having to copy the string
        i.read(const_cast<char*>(result.data()),size);
        return(result);
    } else {
        return(STRING_T());
    }
}

/**
 * Serialize a string where the length is limited to 2^32 chars.
 * The length is stored in front of the size.
 */
void serializeString32(std::ostream& o, const STRING_T& s) {
#ifdef DEBUG
    if(s.size() > 4294967296) {
        std::cout << "Error (serialize_::SerializeHelper::serializeString32): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeInt(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

STRING_T deserializeString32(std::istream& i) {
    UINT32_T size = deserializeInt(i);
    if(size > 0)
    {
        STRING_T result(size,'a');
        // const_cast necessary to avoid overhead by having to copy the string
        i.read(const_cast<char*>(result.data()),size);
        return(result);
    } else {
        return(STRING_T());
    }
}

/**
 * Serialize a string where the length is limited to 2^64 chars.
 * The length is stored in front of the size.
 */
void serializeString64(std::ostream& o, const STRING_T& s) {
    serializeLong(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

STRING_T deserializeString64(std::istream& i) {
    UINT64_T size = deserializeLong(i);
    if(size > 0) {
        STRING_T result(size,'a');
        // const_cast necessary to avoid overhead by having to copy the string
        i.read(const_cast<char*>(result.data()),size);
        return(result);
    } else {
        return(STRING_T());
    }
}

/**
 * Serialize a string where the length is limited to maximall 2^64 chars.
 * The size of the number representing the length is variable.
 */
void serializeString(std::ostream& o, const STRING_T& s) {
    serializeIntVar(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

STRING_T deserializeString(std::istream& i) {
    SIZE_T size = deserializeIntVar<SIZE_T>(i);
    if(size > 0) {
        STRING_T result(size,'a');
        i.read(const_cast<char*>(result.data()),size);
        return(result);
    } else {
        return(STRING_T());
    }
}

template<typename T>
void serializeByteArray(std::ostream& o, const SIZE_T& len, const T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeByte(o,data[i]);
    }
}

template<typename T>
T* deserializeByteArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    T* data = new T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeByte(is);
    }
    return(data);
}

template<typename T>
void serializeShortArray(std::ostream& o, const SIZE_T& len, const T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeShort(o,data[i]);
    }
}

template<typename T>
T* deserializeShortArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    T* data = new T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeShort(is);
    }
    return(data);
}

template<typename T>
void serializeIntArray(std::ostream& o, const SIZE_T& len, const T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeInt(o,data[i]);
    }
}

template<typename T>
T* deserializeIntArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    T* data = new T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeInt(is);
    }
    return(data);
}

template<typename T>
void serializeLongArray(std::ostream& o, const SIZE_T& len, const T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeLong(o,data[i]);
    }
}

template<typename T>
T* deserializeLongArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    T* data = new T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeLong(is);
    }
    return(data);
}

void serializeFloatArray(std::ostream& o, const SIZE_T& len, const FLOAT_T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeFloat(o,data[i]);
    }
}

FLOAT_T* deserializeFloatArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    FLOAT_T* data = new FLOAT_T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeFloat(is);
    }
    return(data);
}

void serializeDoubleArray(std::ostream& o, const SIZE_T& len, const DOUBLE_T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeDouble(o,data[i]);
    }
}

DOUBLE_T* deserializeDoubleArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    DOUBLE_T* data = new DOUBLE_T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeDouble(is);
    }
    return(data);
}

void serializeStringArray(std::ostream& o, const SIZE_T& len, const STRING_T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeString(o,data[i]);
    }
}

STRING_T* deserializeStringArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    STRING_T* data = new STRING_T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeString(is);
    }
    return(data);
}

}}

#endif
