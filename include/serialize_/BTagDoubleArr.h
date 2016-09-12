#ifndef BTC_SERIALIZE_BTAGDOUBLEARR_H
#define BTC_SERIALIZE_BTAGDOUBLEARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

class BTagDoubleArr : public BTagArr<DOUBLE_T> {

  public:
    BTagDoubleArr() : BTagArr<DOUBLE_T>() {}

    BTagDoubleArr(const BTagDoubleArr& bt) : BTagArr<DOUBLE_T>(bt) {}

#ifdef ASSERT_C11
    BTagDoubleArr(BTagDoubleArr&& bt) : BTagArr<DOUBLE_T>(bt) {}
#endif

    BTagDoubleArr(DOUBLE_T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<DOUBLE_T>(value,length,ownership) {}

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
