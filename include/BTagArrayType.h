#ifndef BTC_BTAGARRAYTYPE_H
#define BTC_BTAGARRAYTYPE_H

#include "IBTagBase.h"

namespace BTC {

template<typename T>
class BTagArrayType : public IBTagBase {

  public:
    T* data;
    uint64_t len;
    bool owner;

    BTagArrayType() : data(0), len(0), owner(false) {}

    BTagArrayType(const BTagArrayType<T>& bt) 
            : data(0), len(bt.len), owner(bt.owner) {
        if(owner) {
            // Copy array
            data = new T[len];
            for(size_t i=0; i<len; ++i) {
                data[i] = bt.data[i];
            }
        } else {
            data = bt.data;
        }
    }

#ifdef ASSERT_C11
    BTagArrayType(BTagArrayType<T>&& bt)
            : data(bt.data), len(bt.len), owner(bt.owner) {
        bt.data = 0;
        bt.owner = false;
    }
#endif

    BTagArrayType(T* value, uint64_t length, bool ownership) 
            : data(value), len(length), owner(ownership) {
    }

    ~BTagArrayType() {
        if(owner) {
            delete[] data;
        }
    }

    unsigned char getTypeID() const {
        return DataType<T*>::value;
    }

    void serialize(std::ostream& os) const {
        SerializeHelper::serializeArray<T>(os,len,data);
    }

    void deserialize(std::istream& is) {
        if(owner) {
            delete[] data;
        }
        data = SerializeHelper::deserializeArray<T>(is,len);
        owner = true;
    }

    std::ostream& print(std::ostream& os, uint16_t increment) const {
        os << int(getTypeID()) << ',' << len << ',' << owner;
        return os;
    }
};

}

#endif
