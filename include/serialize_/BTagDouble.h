#ifndef BTC_SERIALIZE_BTAGDOUBLE_H_
#define BTC_SERIALIZE_BTAGDOUBLE_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagDouble : public BTagVal<T> {

  public:
    BTagDouble() : BTagVal<T>() {}
    BTagDouble(const BTagDouble& bt) : BTagVal<T>(bt) {}
    BTagDouble(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::DOUBLE;
    }

    void serialize(std::ostream& os) const {
        serializeDouble(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeDouble(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "d{" << this->data << '}';
        return os;
    }
};

}}

#endif
