#ifndef BTC_SERIALIZE_BTAGSTRING_H_
#define BTC_SERIALIZE_BTAGSTRING_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

class BTagString : public BTagVal<STRING_T> {

  public:
    BTagString() : BTagVal<STRING_T>() {}
    BTagString(const BTagString& bt) : BTagVal<STRING_T>(bt) {}
    BTagString(const STRING_T& value) : BTagVal<STRING_T>(value) {}

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
