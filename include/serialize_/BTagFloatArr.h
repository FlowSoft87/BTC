#ifndef BTC_SERIALIZE_BTAGFLOATARR_H
#define BTC_SERIALIZE_BTAGFLOATARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagFloatArr : public BTagArr<T> {

  public:
    BTagFloatArr() : BTagArr<T>() {}

    BTagFloatArr(const BTagFloatArr& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagFloatArr(BTagFloatArr&& bt) : BTagArr<T>(bt) {}
#endif

    BTagFloatArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::FLOAT_ARR;
    }

    void serialize(std::ostream& os) const {
        serializeFloatArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeFloatArray(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << int(getTypeID()) << ',' << this->len << ',' << this->owner;
        return os;
    }
};

}}

#endif
