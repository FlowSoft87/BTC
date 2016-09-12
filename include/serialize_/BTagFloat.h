#ifndef BTC_SERIALIZE_BTAGFLOAT_H_
#define BTC_SERIALIZE_BTAGFLOAT_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

class BTagFloat : public BTagVal<FLOAT_T> {

  public:
    BTagFloat() : BTagVal<FLOAT_T>() {}
    BTagFloat(const BTagFloat& bt) : BTagVal<FLOAT_T>(bt) {}
    BTagFloat(const FLOAT_T& value) : BTagVal<FLOAT_T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::FLOAT;
    }

    void serialize(std::ostream& os) const {
        serializeFloat(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeFloat(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << int(getTypeID()) << ',' << this->data;
        return os;
    }
};

}}

#endif
