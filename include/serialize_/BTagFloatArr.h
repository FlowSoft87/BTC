#ifndef BTC_SERIALIZE_BTAGFLOATARR_H
#define BTC_SERIALIZE_BTAGFLOATARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

class BTagFloatArr : public BTagArr<FLOAT_T> {

  public:
    BTagFloatArr() : BTagArr<FLOAT_T>() {}

    BTagFloatArr(const BTagFloatArr& bt) : BTagArr<FLOAT_T>(bt) {}

#ifdef ASSERT_C11
    BTagFloatArr(BTagFloatArr&& bt) : BTagArr<FLOAT_T>(bt) {}
#endif

    BTagFloatArr(FLOAT_T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<FLOAT_T>(value,length,ownership) {}

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
