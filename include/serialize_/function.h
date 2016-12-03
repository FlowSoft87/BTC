// TODO Change the serialization of integers: Check for endian 
// and point to function that does nothing if endian is okay. (Flo)

#ifndef BTC_SERIALIZE_FUNCTION_H
#define BTC_SERIALIZE_FUNCTION_H

#include <istream>
#include <ostream>
#include <limits>
#include <cmath>

#include "data_type.h"

namespace BTC {
namespace serialize_ {

// The majority of desktop computers use little endian while ARM (mobile market) 
// seems to use big endian.
// We choose to focus on the server architecture and desktop PCs and choose the
// little endian standard for the internal format of integers!

/**
 * Class that converts host to little endian and back.
 * To map between the two formats it uses a fixed number whose
 * representation in memory is analyzed dynamically.
 * In case of an underlying little-endian architecture the
 * conversion methods do nothing.
 * Otherwise, the byte order is manipulated accordingly.
 * The byte order is checked only once at creation of an
 * object of this class and for later use only a single
 * boolean needs to be checked.
 * This makes the splitting of integers into bytes in the right
 * order obsolete.
 */
class ByteOrder {

    union {
        UINT16_T val;
        UINT8_T bytes[2];
    } short_map;
    union {
        UINT32_T val;
        UINT8_T bytes[4];
    } int_map;
    union {
        UINT64_T val;
        UINT8_T bytes[8];
    } long_map;
    bool littleEndian;

public:
    ByteOrder() : short_map(), int_map(), long_map(), littleEndian(false) {
        short_map.val = 0x0100;
        int_map.val = 0x03020100;
        long_map.val = 0x0706050403020100;
        if (int_map.bytes[0] == 0 &&
            int_map.bytes[1] == 1 &&
            int_map.bytes[2] == 2 &&
            int_map.bytes[3] == 3) {
            littleEndian = true;
        }
    }

    bool isLittleEndian() const {
        return littleEndian;
    }

    void toLittleEndian(UINT16_T& val) const {
        if (!littleEndian) {
            UINT16_T temp = val;
            UINT8_T* arr = reinterpret_cast<UINT8_T*>(&val);
            UINT8_T* arr_temp = reinterpret_cast<UINT8_T*>(&temp);
            arr[short_map.bytes[0]] = arr_temp[0];
            arr[short_map.bytes[1]] = arr_temp[1];
        }
    }

    void toLittleEndian(UINT32_T& val) const {
        if (!littleEndian) {
            UINT32_T temp = val;
            UINT8_T* arr = reinterpret_cast<UINT8_T*>(&val);
            UINT8_T* arr_temp = reinterpret_cast<UINT8_T*>(&temp);
            arr[int_map.bytes[0]] = arr_temp[0];
            arr[int_map.bytes[1]] = arr_temp[1];
            arr[int_map.bytes[2]] = arr_temp[2];
            arr[int_map.bytes[3]] = arr_temp[3];
        }
    }

    void toLittleEndian(UINT64_T& val) const {
        if (!littleEndian) {
            UINT64_T temp = val;
            UINT8_T* arr = reinterpret_cast<UINT8_T*>(&val);
            UINT8_T* arr_temp = reinterpret_cast<UINT8_T*>(&temp);
            arr[long_map.bytes[0]] = arr_temp[0];
            arr[long_map.bytes[1]] = arr_temp[1];
            arr[long_map.bytes[2]] = arr_temp[2];
            arr[long_map.bytes[3]] = arr_temp[3];
            arr[long_map.bytes[4]] = arr_temp[4];
            arr[long_map.bytes[5]] = arr_temp[5];
            arr[long_map.bytes[6]] = arr_temp[6];
            arr[long_map.bytes[7]] = arr_temp[7];
        }
    }

    void toHostEndian(UINT16_T& val) const {
        if (!littleEndian) {
            UINT8_T* arr = reinterpret_cast<UINT8_T*>(&val);
            UINT8_T temp = arr[short_map.bytes[1]];
            arr[0] = arr[short_map.bytes[0]];
            arr[1] = temp;
        }
    }

    void toHostEndian(UINT32_T& val) const {
        if (!littleEndian) {
            UINT32_T temp = val;
            UINT8_T* arr = reinterpret_cast<UINT8_T*>(&val);
            UINT8_T* arr_temp = reinterpret_cast<UINT8_T*>(&temp);
            arr[0] = arr_temp[int_map.bytes[0]];
            arr[1] = arr_temp[int_map.bytes[1]];
            arr[2] = arr_temp[int_map.bytes[2]];
            arr[3] = arr_temp[int_map.bytes[3]];
        }
    }

    void toHostEndian(UINT64_T& val) const {
        if (!littleEndian) {
            UINT64_T temp = val;
            UINT8_T* arr = reinterpret_cast<UINT8_T*>(&val);
            UINT8_T* arr_temp = reinterpret_cast<UINT8_T*>(&temp);
            arr[0] = arr_temp[long_map.bytes[0]];
            arr[1] = arr_temp[long_map.bytes[1]];
            arr[2] = arr_temp[long_map.bytes[2]];
            arr[3] = arr_temp[long_map.bytes[3]];
            arr[4] = arr_temp[long_map.bytes[4]];
            arr[5] = arr_temp[long_map.bytes[5]];
            arr[6] = arr_temp[long_map.bytes[6]];
            arr[7] = arr_temp[long_map.bytes[7]];
        }
    }

};

static const ByteOrder byte_order;

/**
 * Serialize a single byte.
 * Requirement is that the number is castable to BTC::UINT8_T.
 */
inline void serializeByte(std::ostream& o, UINT8_T b) {
    o.write(reinterpret_cast<const char*>(&b),1);
}

inline UINT8_T deserializeByte(std::istream& i) {
    UINT8_T data = 0;
    i.read(reinterpret_cast<char*>(&data),1);
    return(data);
}

/**
 * Serialize an unsigned integer containing data of up to 2 byte.
 * Integer has to fulfill (s < 2^16) and have integer division and
 * modulo operations defined.
 */
//template<typename UINT16_T>
inline void serializeShort(std::ostream& o, UINT16_T s) {
    byte_order.toLittleEndian(s);
    o.write(reinterpret_cast<const char*>(&s),2);
}

inline UINT16_T deserializeShort(std::istream& i) {
    UINT16_T data;
    i.read(reinterpret_cast<char*>(&data),2);
    //data += (UINT16_T(deserializeByte(i)))*256u;
    //data += deserializeByte(i);
    byte_order.toHostEndian(data);
    return(data);
}

/**
 * Serialize an unsigned integer containing data of up to 4 byte.
 * Integer has to fulfill (s < 2^32) and have integer division and
 * modulo operations defined.
 */
//template<typename UINT32_T>
inline void serializeInt(std::ostream& o, UINT32_T i) {
    byte_order.toLittleEndian(i);
    o.write(reinterpret_cast<const char*>(&i),4);
    //UINT16_T data = i/65536u;
    //serializeShort(o,data);
    //data = i%65536u;
    //serializeShort(o,data);
}

inline UINT32_T deserializeInt(std::istream& i) {
    UINT32_T data;
    i.read(reinterpret_cast<char*>(&data),4);
    byte_order.toHostEndian(data);
    //data += (UINT32_T(deserializeShort(i)))*65536u;
    //data += deserializeShort(i);
    return(data);
}

/**
 * Serialize an unsigned integer containing data of up to 8 byte.
 * Integer has to fulfill (s < 2^64) and have integer division and
 * modulo operations defined.
 */
//template<typename UINT64_T>
inline void serializeLong(std::ostream& o, UINT64_T i) {
    byte_order.toLittleEndian(i);
    o.write(reinterpret_cast<const char*>(&i),8);
    //UINT32_T data = i/4294967296u;
    //serializeInt(o,data);
    //data = i%4294967296u;
    //serializeInt(o,data);
}

inline UINT64_T deserializeLong(std::istream& i) {
    UINT64_T data;
    i.read(reinterpret_cast<char*>(&data),8);
    byte_order.toHostEndian(data);
    //data += (UINT64_T(deserializeInt(i)))*4294967296u;
    //data += deserializeInt(i);
    return(data);
}

/**
 * Serialize an arbitrary integer of maximum size of 8 byte.
 * Depending on how large the number is the correct representation is chosen.
 * For this purpose an extra byte is stored in front of the number which tells 
 * of what type the representation is.
 */
template<typename T>
void serializeIntVar(std::ostream& o, const T& i) {
    if(i <= 256u) {
        serializeByte(o,0);
        serializeByte(o,i);
    } else if((i > 256u) && (i <= 65536u)) {
        serializeByte(o,1);
        serializeShort(o,i);
    } else if((i > 65536u) && (i <= 4294967296u)) {
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
 * Get the byte-size for an int variable with unspecified size.
 */
template<typename T>
SIZE_T getIntVarByteSize(const T& val) {
    if(val <= 256u) {
        return 2;
    } else if((val > 256u) && (val <= 65536u)) {
        return 3;
    } else if((val > 65536u) && (val <= 4294967296u)) {
        return 5;
    }
    return 9;
}

/**
 * Serialize a floating point number of size 4 byte.
 * The number is decomposed numerically and stored in a uint32_t variable
 * in the format |s|exp|mant| where s is 1 bit, exp is 8 bits and mant
 * 23 bits long.
 * It is then serialized as a uint32_t to get rid of byte order issues.
 */
inline void serializeFloat(std::ostream& os, const FLOAT_T& val) {
    UINT32_T data = 0;
    int exp = 0;
    FLOAT_T mant = std::frexp(val,&exp);
    // Write sign
    if (mant < 0) {
        mant = -mant;
        data += 2147483648u;  // Set bit 31: 2^31
    }
    exp += 127;
    // Special cases
    if (val == 0) {
        mant = 0.5f;
        exp = 255;
    } else if(std::isinf(val)) {
        // Handle inf case
        mant = 0.75f;
        exp = 255;
    } else if(std::isnan(val)) {
        // Handle quiet NaN case
        mant = 0.875f;
        exp = 255;
    }
    // Write exponent
    data += (UINT32_T(exp))*8388608u;  // Set bits 23-30: 2^23
    // Write mantissa
    mant -= 0.5f;  // Subtract hidden bit
    data += UINT32_T(mant*16777216.f);  // Set bits 0-22: (0 <= mant < 0.5)
    serializeInt(os,data);
}

/**
 * Deserialize a floating point number of size 4 byte.
 */
inline FLOAT_T deserializeFloat(std::istream& is) {
    UINT32_T data = deserializeInt(is);
    FLOAT_T val = 0.5f;
    // Get sign
    bool s = data/2147483648u;
    data %= 2147483648u;
    // Get exponent
    int exp = data/8388608u;
    data %= 8388608u;
    // Get mantissa
    val += (FLOAT_T(data))/16777216.f;
    // Special cases
    if(val == 0.5f && exp == 255) {
        // Handle zero case
        if(s) {
            val = -0.f;
        } else {
            val = 0.f;
        }
        return val;
    } else if (val == 0.75f && exp == 255) {
        // Handle inf case
        if(s) {
            val = -std::numeric_limits<FLOAT_T>::infinity();
        } else {
            val = std::numeric_limits<FLOAT_T>::infinity();
        }
        return val;
    } else if (val == 0.875f && exp == 255) {
        // Handle quiet NaN case
        if (s) {
            val = -nanf("");
        } else {
            val = nanf("");
        }
        return val;
    }
    // Normal number
    if (s) {
        val = -val;
    }
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
inline void serializeDouble(std::ostream& os, const DOUBLE_T& val) {
    UINT64_T data = 0;
    int exp = 0;
    DOUBLE_T mant = std::frexp(val,&exp);  // Split double into mant and exp: val = mant*2^exp
    // Write sign
    if (mant < 0) {
        mant = -mant;
        data += 9223372036854775808ul;  // Set bit 63: 2^63
    }
    exp += 1023;
    // Special cases
    if (val == 0) {
        mant = 0.5;
        exp = 2047;
    } else if(std::isinf(val)) {
        // Handle inf case
        mant = 0.75;
        exp = 2047;
    } else if(std::isnan(val)) {
        // Handle quiet NaN case
        mant = 0.875;
        exp = 2047;
    }
    // Write exponent
    data += (UINT64_T(exp))*4503599627370496ul;  // Set bits 52-62: 2^52
    // Write mantissa
    mant -= 0.5;  // Subtract hidden bit
    data += UINT64_T(mant*9007199254740992.);  // Set bits 0-51: (0 <= mant < 0.5)
    serializeLong(os,data);
}

/**
 * Deserialize a floating point number of size 8 byte.
 */
inline DOUBLE_T deserializeDouble(std::istream& is) {
    UINT64_T data = deserializeLong(is);
    DOUBLE_T val = 0.5;
    // Get sign
    bool s = data/9223372036854775808ul;
    data %= 9223372036854775808ul;
    // Get exponent
    int exp = data/4503599627370496ul;
    data %= 4503599627370496ul;
    // Get mantissa
    val += (DOUBLE_T(data))/9007199254740992.;
    if(val == 0.5 && exp == 2047) {
        // Handle zero case
        if(s) {
            val = -0.;
        } else {
            val = 0.;
        }
        return val;
    } else if(val == 0.75 && exp == 2047) {
        // Handle inf case
        if(s) {
            val = -std::numeric_limits<DOUBLE_T>::infinity();
        } else {
            val = std::numeric_limits<DOUBLE_T>::infinity();
        }
        return val;
    } else if(val == 0.875 && exp == 2047) {
        // Handle quiet NaN case
        if (s) {
            val = -nan("");
        } else {
            val = nan("");
        }
        return val;
    }
    // Normal number
    if (s) {
        val = -val;
    }
    exp -= 1023;
    // Multiply them together
    val = std::ldexp(val,exp);
    return(val);
}

/**
 * Serialize a string where the length is limited to 2^8 chars.
 * The length is stored in front of the string.
 */
inline void serializeString8(std::ostream& o, const STRING_T& s) {
#ifdef DEBUG
    if(s.size() > 256u) {
        std::cout << "Error (serialize_::SerializeHelper::serializeString8): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeByte(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

inline STRING_T deserializeString8(std::istream& i) {
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
inline void serializeString16(std::ostream& o, const STRING_T& s) {
#ifdef DEBUG
    if(s.size() > 65536u) {
        std::cout << "Error (serialize_::SerializeHelper::serializeString16): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeShort(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

inline STRING_T deserializeString16(std::istream& i) {
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
inline void serializeString32(std::ostream& o, const STRING_T& s) {
#ifdef DEBUG
    if(s.size() > 4294967296ul) {
        std::cout << "Error (serialize_::SerializeHelper::serializeString32): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeInt(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

inline STRING_T deserializeString32(std::istream& i) {
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
inline void serializeString64(std::ostream& o, const STRING_T& s) {
    serializeLong(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

inline STRING_T deserializeString64(std::istream& i) {
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
inline void serializeString(std::ostream& o, const STRING_T& s) {
    serializeIntVar(o,s.size());
    if(s.size() > 0) {
        o.write(s.data(),s.size());
    }
}

inline STRING_T deserializeString(std::istream& i) {
    SIZE_T size = deserializeIntVar<SIZE_T>(i);
    if(size > 0) {
        STRING_T result(size,'a');
        i.read(const_cast<char*>(result.data()),size);
        return(result);
    } else {
        return(STRING_T());
    }
}

/**
 * Get bytesize of a string with unspecified size.
 */
inline SIZE_T getStringByteSize(const STRING_T& s) {
    SIZE_T bytesize = s.size();
    bytesize += getIntVarByteSize(bytesize);
    return bytesize;
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

inline void serializeFloatArray(std::ostream& o, const SIZE_T& len, const FLOAT_T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeFloat(o,data[i]);
    }
}

inline FLOAT_T* deserializeFloatArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    FLOAT_T* data = new FLOAT_T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeFloat(is);
    }
    return(data);
}

inline void serializeDoubleArray(std::ostream& o, const SIZE_T& len, const DOUBLE_T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeDouble(o,data[i]);
    }
}

inline DOUBLE_T* deserializeDoubleArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    DOUBLE_T* data = new DOUBLE_T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeDouble(is);
    }
    return(data);
}

inline void serializeStringArray(std::ostream& o, const SIZE_T& len, const STRING_T* data) {
    serializeIntVar(o,len);
    for(SIZE_T i=0; i<len; ++i) {
        serializeString(o,data[i]);
    }
}

inline STRING_T* deserializeStringArray(std::istream& is, SIZE_T& len) {
    len = deserializeIntVar<SIZE_T>(is);
    STRING_T* data = new STRING_T[len];
    for(SIZE_T i=0; i<len; ++i) {
        data[i] = deserializeString(is);
    }
    return(data);
}

}}

#endif
