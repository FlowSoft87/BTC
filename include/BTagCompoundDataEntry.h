#ifndef BTC_BTAGCOMPOUNDDATAENTRY_H_
#define BTC_BTAGCOMPOUNDDATAENTRY_H_

namespace BTC {

struct BTagCompoundDataEntry {
    std::string tag;
    ptr_::SharedObjPtr<IBTagBase> data;

    BTagCompoundDataEntry()
            : tag(), data() {
    }

    BTagCompoundDataEntry(const std::string& t, ptr_::SharedObjPtr<IBTagBase> d)
            : tag(t), data(d) {
    }
};

}

#endif
