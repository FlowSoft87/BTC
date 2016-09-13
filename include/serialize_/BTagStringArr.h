#ifndef BTC_SERIALIZE_BTAGSTRINGARR_H
#define BTC_SERIALIZE_BTAGSTRINGARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagStringArr : public BTagArr<T> {

  public:
    BTagStringArr() : BTagArr<T>() {}

    BTagStringArr(const BTagStringArr& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagStringArr(BTagStringArr&& bt) : BTagArr<T>(bt) {}
#endif

    BTagStringArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::STRING_ARR;
    }

    void serialize(std::ostream& os) const {
        serializeStringArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeStringArray(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << int(getTypeID()) << ',' << this->len << ',' << this->owner;
        return os;
    }
};

}}

#endif
