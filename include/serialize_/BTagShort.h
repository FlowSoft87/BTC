#ifndef BTC_SERIALIZE_BTAGSHORT_H_
#define BTC_SERIALIZE_BTAGSHORT_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagShort : public BTagVal<T> {

  public:
    BTagShort() : BTagVal<T>() {}
    BTagShort(const BTagShort<T>& bt) : BTagVal<T>(bt) {}
    BTagShort(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT16;
    }

    void serialize(std::ostream& os) const {
        serializeShort(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeShort(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << int(getTypeID()) << ',' << this->data;
        return os;
    }
};

}}

#endif
