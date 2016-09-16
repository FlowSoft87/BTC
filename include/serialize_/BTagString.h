#ifndef BTC_SERIALIZE_BTAGSTRING_H_
#define BTC_SERIALIZE_BTAGSTRING_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagString : public BTagVal<T> {

  public:
    BTagString() : BTagVal<T>() {}
    BTagString(const BTagString& bt) : BTagVal<T>(bt) {}
    BTagString(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::STRING;
    }

    void serialize(std::ostream& os) const {
        serializeString(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeString(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << int(getTypeID()) << ',' << this->data;
        return os;
    }
};

}}

#endif
