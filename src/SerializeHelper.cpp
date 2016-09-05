#include "SerializeHelper.h"

namespace BTC {

void SerializeHelper::serializeString8(std::ostream& o, const std::string& s)
{
#ifdef DEBUG
    if(s.size() > std::numeric_limits<uint8_t>::max())
    {
        std::cout << "Error (serialize_::SerializeHelper::serializeString32): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeByte(o,s.size());
    if(s.size() > 0) o.write(s.data(),s.size());
}

void SerializeHelper::serializeString16(std::ostream& o, const std::string& s)
{
#ifdef DEBUG
    if(s.size() > std::numeric_limits<uint16_t>::max())
    {
        std::cout << "Error (serialize_::SerializeHelper::serializeString16): Not a short string!" << std::endl;
        exit(1);
    }
#endif
    serializeShort(o,s.size());
    if(s.size() > 0) o.write(s.data(),s.size());
}

void SerializeHelper::serializeString32(std::ostream& o, const std::string& s)
{
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

void SerializeHelper::serializeString64(std::ostream& o, const std::string& s)
{
    serializeLong(o,s.size());
    if(s.size() > 0) o.write(s.data(),s.size());
}

void SerializeHelper::serializeString(std::ostream& o, const std::string& s)
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

std::string SerializeHelper::deserializeString8(std::istream& i)
{
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

std::string SerializeHelper::deserializeString16(std::istream& i)
{
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

std::string SerializeHelper::deserializeString32(std::istream& i)
{
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

std::string SerializeHelper::deserializeString64(std::istream& i)
{
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

std::string SerializeHelper::deserializeString(std::istream& i)
{
    /*uint8_t type = deserializeByte(i);
    if(type == 0) return(deserializeString8(i));
    else if(type == 1) return(deserializeString16(i));
    else if(type == 2) return(deserializeString32(i));
    else return(deserializeString64(i));*/
    uint64_t size = deserializeIntVar<uint64_t>(i);
    if(size > 0)
    {
        std::string result(size,'a');
        i.read(const_cast<char*>(result.data()),size);
        return(result);
    }
    else return(std::string());
}

void SerializeHelper::serializeByte(std::ostream& o, uint8_t b)
{
    o.write(reinterpret_cast<const char*>(&b),sizeof(uint8_t));
}

uint8_t SerializeHelper::deserializeByte(std::istream& i)
{
    uint8_t data = 0;
    i.read(reinterpret_cast<char*>(&data),sizeof(uint8_t));
    return(data);
}

void SerializeHelper::serializeShort(std::ostream& o, uint16_t s)
{
    uint16_t data = htons(s);
    o.write(reinterpret_cast<const char*>(&data),sizeof(uint16_t));
}

uint16_t SerializeHelper::deserializeShort(std::istream& i)
{
    uint16_t data = 0;
    i.read(reinterpret_cast<char*>(&data),sizeof(uint16_t));
    return(ntohs(data));
}

void SerializeHelper::serializeInt(std::ostream& o, uint32_t i)
{
    uint32_t data = htonl(i);
    o.write(reinterpret_cast<const char*>(&data),sizeof(uint32_t));
}

uint32_t SerializeHelper::deserializeInt(std::istream& i)
{
    uint32_t data = 0;
    i.read(reinterpret_cast<char*>(&data),sizeof(uint32_t));
    return(ntohl(data));
}

void SerializeHelper::serializeLong(std::ostream& o, uint64_t i)
{
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

uint64_t SerializeHelper::deserializeLong(std::istream& i)
{
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

void SerializeHelper::serializeFloat(std::ostream& os, float val)
{
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

float SerializeHelper::deserializeFloat(std::istream& is)
{
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

void SerializeHelper::serializeDouble(std::ostream& os, double val)
{
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

double SerializeHelper::deserializeDouble(std::istream& is)
{
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

void SerializeHelper::serializeByteArray(std::ostream& o, uint64_t len, const uint8_t* data)
{
    serializeIntVar(o,len);
    for(size_t i=0; i<len; ++i) serializeByte(o,data[i]);
//    o.write(reinterpret_cast<const char*>(&data),sizeof(uint8_t)*len);
}

uint8_t* SerializeHelper::deserializeByteArray(std::istream& is, uint64_t& len)
{
    len = deserializeIntVar<uint64_t>(is);
    uint8_t* data = new uint8_t[len];
    for(size_t i=0; i<len; ++i) data[i] = deserializeByte(is);
    //is.read(reinterpret_cast<char*>(&data),sizeof(uint8_t)*len);
    return(data);
}

void SerializeHelper::serializeShortArray(std::ostream& o, uint64_t len, const uint16_t* data)
{
    serializeIntVar(o,len);
    for(size_t i=0; i<len; ++i) serializeShort(o,data[i]);
}

uint16_t* SerializeHelper::deserializeShortArray(std::istream& is, uint64_t& len)
{
    len = deserializeIntVar<uint64_t>(is);
    uint16_t* data = new uint16_t[len];
    for(size_t i=0; i<len; ++i) data[i] = deserializeShort(is);
    return(data);
}

void SerializeHelper::serializeIntArray(std::ostream& o, uint64_t len, const uint32_t* data)
{
    serializeIntVar(o,len);
    for(size_t i=0; i<len; ++i) serializeInt(o,data[i]);
}

uint32_t* SerializeHelper::deserializeIntArray(std::istream& is, uint64_t& len)
{
    len = deserializeIntVar<uint64_t>(is);
    uint32_t* data = new uint32_t[len];
    for(size_t i=0; i<len; ++i) data[i] = deserializeInt(is);
    return(data);
}

void SerializeHelper::serializeLongArray(std::ostream& o, uint64_t len, const uint64_t* data)
{
    serializeIntVar(o,len);
    for(size_t i=0; i<len; ++i) serializeLong(o,data[i]);
}

uint64_t* SerializeHelper::deserializeLongArray(std::istream& is, uint64_t& len)
{
    len = deserializeIntVar<uint64_t>(is);
    uint64_t* data = new uint64_t[len];
    for(size_t i=0; i<len; ++i) data[i] = deserializeLong(is);
    return(data);
}

void SerializeHelper::serializeFloatArray(std::ostream& o, uint64_t len, const float* data)
{
    serializeIntVar(o,len);
    for(size_t i=0; i<len; ++i) serializeFloat(o,data[i]);
}

float* SerializeHelper::deserializeFloatArray(std::istream& is, uint64_t& len)
{
    len = deserializeIntVar<uint64_t>(is);
    float* data = new float[len];
    for(size_t i=0; i<len; ++i) data[i] = deserializeFloat(is);
    return(data);
}

void SerializeHelper::serializeDoubleArray(std::ostream& o, uint64_t len, const double* data)
{
    serializeIntVar(o,len);
    for(size_t i=0; i<len; ++i) serializeDouble(o,data[i]);
}

double* SerializeHelper::deserializeDoubleArray(std::istream& is, uint64_t& len)
{
    len = deserializeIntVar<uint64_t>(is);
    double* data = new double[len];
    for(size_t i=0; i<len; ++i) data[i] = deserializeDouble(is);
    return(data);
}

template<>
void SerializeHelper::serialize(std::ostream& os, const std::string& i)
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

