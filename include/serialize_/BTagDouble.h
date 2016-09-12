#ifndef BTC_SERIALIZE_BTAGDOUBLE_H_
#define BTC_SERIALIZE_BTAGDOUBLE_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

class BTagDouble : public BTagVal<DOUBLE_T> {

  public:
    BTagDouble() : BTagVal<DOUBLE_T>() {}
    BTagDouble(const BTagDouble& bt) : BTagVal<DOUBLE_T>(bt) {}
    BTagDouble(const DOUBLE_T& value) : BTagVal<DOUBLE_T>(value) {}

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
        os << int(getTypeID()) << ',' << this->data;
        return os;
    }
};

}}

#endif
