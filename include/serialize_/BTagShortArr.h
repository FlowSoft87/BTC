#ifndef BTC_SERIALIZE_BTAGSHORTARR_H
#define BTC_SERIALIZE_BTAGSHORTARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagShortArr : public BTagArr<T> {

  public:
    BTagShortArr() : BTagArr<T>() {}

    BTagShortArr(const BTagShortArr<T>& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagShortArr(BTagShortArr<T>&& bt) : BTagArr<T>(bt) {}
#endif

    BTagShortArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT16_ARR;
    }

    void serialize(std::ostream& os) const {
        serializeShortArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeShortArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "sa{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

}}

#endif
