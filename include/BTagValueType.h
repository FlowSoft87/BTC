#ifndef BTC_BTAGVALUETYPE_H_
#define BTC_BTAGVALUETYPE_H_

#include "SerializeHelper.h"
#include "IBTagBase.h"

namespace BTC {

template<typename T>
class BTagValueType : public IBTagBase {

  public:
    T data;

    BTagValueType() : data() {}
    BTagValueType(const T& value) : data(value) {}

    unsigned char getTypeID() const {
        return DataType<T>::value;
    }

    void serialize(std::ostream& os) const {
        SerializeHelper::serialize<T>(os,data);
    }

    void deserialize(std::istream& is) {
        data = SerializeHelper::deserialize<T>(is);
    }

    std::ostream& print(std::ostream& os, uint16_t increment) const {
        os << int(getTypeID()) << ',' << data;
        return os;
    }
};

}

#endif
