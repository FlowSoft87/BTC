#ifndef BTC_SERIALIZE_BTAGINTARR_H
#define BTC_SERIALIZE_BTAGINTARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagIntArr : public BTagArr<T> {

  public:
    BTagIntArr() : BTagArr<T>() {}

    BTagIntArr(const BTagIntArr<T>& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagIntArr(BTagIntArr<T>&& bt) : BTagArr<T>(bt) {}
#endif

    BTagIntArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT32_ARR;
    }

    void serialize(std::ostream& os) const {
        serializeIntArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeIntArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "ia{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

}}

#endif
