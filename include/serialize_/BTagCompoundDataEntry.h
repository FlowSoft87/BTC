#ifndef BTC_BTAGCOMPOUNDDATAENTRY_H_
#define BTC_BTAGCOMPOUNDDATAENTRY_H_

namespace BTC {
namespace serialize_ {

struct BTagCompoundDataEntry {
    STRING_T tag;
    ptr_::SharedObjPtr<IBTagBase> data;

    BTagCompoundDataEntry()
            : tag(), data() {
    }

    BTagCompoundDataEntry(const STRING_T& t, ptr_::SharedObjPtr<IBTagBase> d)
            : tag(t), data(d) {
    }
};

}}

#endif
