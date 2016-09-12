#ifndef BTC_SERIALIZE_BTAGLONGARR_H
#define BTC_SERIALIZE_BTAGLONGARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagLongArr : public BTagArr<T> {

  public:
    BTagLongArr() : BTagArr<T>() {}

    BTagLongArr(const BTagLongArr<T>& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagLongArr(BTagLongArr<T>&& bt) : BTagArr<T>(bt) {}
#endif

    BTagLongArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT64_ARR;
    }

    void serialize(std::ostream& os) const {
        serializeLongArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeLongArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << int(getTypeID()) << ',' << this->len << ',' << this->owner;
        return os;
    }
};

}}

#endif
