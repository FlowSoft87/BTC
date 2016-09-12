#ifndef BTC_SERIALIZE_BTAGVAL_H_
#define BTC_SERIALIZE_BTAGVAL_H_

#include "IBTagBase.h"

namespace BTC {
namespace serialize_ {

template<typename T>
class BTagVal : public IBTagBase {
    
  public:
    T data;

    BTagVal() : data() {}
    BTagVal(const BTagVal<T>& bt) : data(0) {}
#ifdef ASSERT_C11
    BTagVal(BTagVal<T>&& bt) : data(bt.data) {}
#endif
    BTagVal(const T& value) : data(value) {}
};

}}

#endif
