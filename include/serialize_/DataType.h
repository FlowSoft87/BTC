#ifndef BTC_SERIALIZE_DATATYPE_H
#define BTC_SERIALIZE_DATATYPE_H

namespace BTC {
namespace serialize_ {

#ifndef ASSERT_C11
typedef size_t SIZE_T;
typedef unsigned char UINT8_T;
typedef unsigned short int UINT16_T;
typedef unsigned long int UINT32_T;
typedef unsigned long long int UINT64_T;
typedef float FLOAT_T;
typedef double DOUBLE_T;
typedef std::string STRING_T;
#endif

#ifdef ASSERT_C11
typedef size_t SIZE_T;
typedef uint8_t UINT8_T;
typedef uint16_t UINT16_T;
typedef uint32_t UINT32_T;
typedef uint64_t UINT64_T;
typedef float FLOAT_T;
typedef double DOUBLE_T;
typedef std::string STRING_T;
#endif

namespace DataTypeID {
static const unsigned char COMPOUND = 0;
static const unsigned char STRING = 1;
static const unsigned char UINT8 = 2;
static const unsigned char UINT16 = 3;
static const unsigned char UINT32 = 4;
static const unsigned char UINT64 = 5;
static const unsigned char FLOAT = 6;
static const unsigned char DOUBLE = 7;
static const unsigned char STRING_ARR = 64;
static const unsigned char UINT8_ARR = 65;
static const unsigned char UINT16_ARR = 66;
static const unsigned char UINT32_ARR = 67;
static const unsigned char UINT64_ARR = 68;
static const unsigned char FLOAT_ARR = 69;
static const unsigned char DOUBLE_ARR = 70;
}

/*template<typename T> struct DataType { static const unsigned char value = 255; };
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
template<> struct DataType<double*> { static const unsigned char value = 70; };*/

}}

#endif
