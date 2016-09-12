#ifndef BTC_SERIALIZE_BTAGSTRINGARR_H
#define BTC_SERIALIZE_BTAGSTRINGARR_H

#include "function.h"
#include "BTagArr.h"

namespace BTC {
namespace serialize_ {

class BTagStringArr : public BTagArr<STRING_T> {

  public:
    BTagStringArr() : BTagArr<STRING_T>() {}

    BTagStringArr(const BTagStringArr& bt) : BTagArr<STRING_T>(bt) {}

#ifdef ASSERT_C11
    BTagStringArr(BTagStringArr&& bt) : BTagArr<STRING_T>(bt) {}
#endif

    BTagStringArr(STRING_T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<STRING_T>(value,length,ownership) {}

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
