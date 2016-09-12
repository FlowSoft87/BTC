#ifndef BTC_SERIALIZE_BTAGBYTEARR_H
#define BTC_SERIALIZE_BTAGBYTEARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagByteArr : public BTagArr<T> {

  public:
    BTagByteArr() : BTagArr<T>() {}
    BTagByteArr(const BTagByteArr<T>& bt) : BTagArr<T>(bt) {}
#ifdef ASSERT_C11
    BTagByteArr(BTagByteArr<T>&& bt) : BTagArr<T>(bt) {}
#endif
    BTagByteArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT8_ARR;
    }

    void serialize(std::ostream& os) const {
        serializeByteArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeByteArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << int(getTypeID()) << ',' << this->len << ',' << this->owner;
        return os;
    }
};

}}

#endif
