#ifndef BTC_BTAGCOMPOUNDENTRY_H
#define BTC_BTAGCOMPOUNDENTRY_H

#include <string>

namespace BTC {

class BTagCompoundEntry
{
public:
    std::string tag;
    size_t position;

    bool operator<(const BTagCompoundEntry& entry) const {
        return(tag.compare(entry.tag) < 0);
    }
};

}

#endif
