#ifndef BTC_SERIALIZE_BTAGLONG_H_
#define BTC_SERIALIZE_BTAGLONG_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagLong : public BTagVal<T> {

  public:
    BTagLong() : BTagVal<T>() {}
    BTagLong(const BTagLong<T>& bt) : BTagVal<T>(bt) {}
    BTagLong(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT64;
    }

    void serialize(std::ostream& os) const {
        serializeLong(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeLong(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << int(getTypeID()) << ',' << this->data;
        return os;
    }
};

}}

#endif
