#ifndef BTC_SERIALIZE_BTAGBYTE_H_
#define BTC_SERIALIZE_BTAGBYTE_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagByte : public BTagVal<T> {

  public:
    BTagByte() : BTagVal<T>() {}
    BTagByte(const BTagByte<T>& bt) : BTagVal<T>(bt) {}
    BTagByte(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT8;
    }

    void serialize(std::ostream& os) const {
        serializeByte(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeByte(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << int(getTypeID()) << ',' << this->data;
        return os;
    }
};

}}

#endif
