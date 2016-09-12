#ifndef BTC_SERIALIZE_BTAGARR_H_
#define BTC_SERIALIZE_BTAGARR_H_

#include "IBTagBase.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagArr : public IBTagBase {
    
  public:
    T* data;
    SIZE_T len;
    bool owner;

    BTagArr() : data(0), len(0), owner(false) {}

    BTagArr(const BTagArr<T>& bt) 
            : data(0), len(bt.len), owner(bt.owner) {
        if(owner) {
            // Copy array
            data = new T[len];
            for(SIZE_T i=0; i<len; ++i) {
                data[i] = bt.data[i];
            }
        } else {
            data = bt.data;
        }
    }

#ifdef ASSERT_C11
    BTagArr(BTagArr<T>&& bt)
            : data(bt.data), len(bt.len), owner(bt.owner) {
        bt.data = 0;
        bt.owner = false;
    }
#endif

    BTagArr(T* value, const SIZE_T& length, bool ownership) 
            : data(value), len(length), owner(ownership) {
    }

    ~BTagArr() {
        if(owner) {
            delete[] data;
        }
    }
};

}}

#endif
