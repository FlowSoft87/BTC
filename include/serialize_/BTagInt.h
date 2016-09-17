#ifndef BTC_SERIALIZE_BTAGINT_H_
#define BTC_SERIALIZE_BTAGINT_H_

#include "function.h"
#include "BTagVal.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagInt : public BTagVal<T> {

  public:
    BTagInt() : BTagVal<T>() {}
    BTagInt(const BTagInt<T>& bt) : BTagVal<T>(bt) {}
    BTagInt(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT32;
    }

    void serialize(std::ostream& os) const {
        serializeInt(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeInt(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "i{" << this->data << '}';
        return os;
    }
};

}}

#endif
