#ifndef BTC_DATATYPE_H
#define BTC_DATATYPE_H

namespace BTC {

class BTagCompound;

/*enum class DataType : uint8_t
{
    COMPOUND = 0,
    STRING = 1,
    BYTE = 2,
    INT16 = 3,
    INT32 = 4,
    INT64 = 5,
    FLOAT = 6,
    DOUBLE = 7,
    STRING_ARR = 64,
    BYTE_ARR = 65,
    INT16_ARR = 66,
    INT32_ARR = 67,
    INT64_ARR = 68,
    FLOAT_ARR = 69,
    DOUBLE_ARR = 70
};*/

template<typename T> struct DataType { static const unsigned char value = 255; };
template<> struct DataType<BTagCompound> { static const unsigned char value = 0; };
template<> struct DataType<std::string> { static const unsigned char value = 1; };
template<> struct DataType<uint8_t> { static const unsigned char value = 2; };
template<> struct DataType<uint16_t> { static const unsigned char value = 3; };
template<> struct DataType<uint32_t> { static const unsigned char value = 4; };
template<> struct DataType<uint64_t> { static const unsigned char value = 5; };
template<> struct DataType<float> { static const unsigned char value = 6; };
template<> struct DataType<double> { static const unsigned char value = 7; };
template<> struct DataType<std::string*> { static const unsigned char value = 64; };
template<> struct DataType<uint8_t*> { static const unsigned char value = 65; };
template<> struct DataType<uint16_t*> { static const unsigned char value = 66; };
template<> struct DataType<uint32_t*> { static const unsigned char value = 67; };
template<> struct DataType<uint64_t*> { static const unsigned char value = 68; };
template<> struct DataType<float*> { static const unsigned char value = 69; };
template<> struct DataType<double*> { static const unsigned char value = 70; };

}

#endif
