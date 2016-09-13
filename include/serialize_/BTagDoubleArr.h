#ifndef BTC_SERIALIZE_BTAGDOUBLEARR_H
#define BTC_SERIALIZE_BTAGDOUBLEARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagDoubleArr : public BTagArr<T> {

  public:
    BTagDoubleArr() : BTagArr<T>() {}

    BTagDoubleArr(const BTagDoubleArr& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagDoubleArr(BTagDoubleArr&& bt) : BTagArr<T>(bt) {}
#endif

    BTagDoubleArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::DOUBLE_ARR;
    }

    void serialize(std::ostream& os) const {
        serializeDoubleArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeDoubleArray(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << int(getTypeID()) << ',' << this->len << ',' << this->owner;
        return os;
    }
};

}}

#endif
