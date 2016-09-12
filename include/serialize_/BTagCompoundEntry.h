#ifndef BTC_SERIALIZE_BTAGCOMPOUNDENTRY_H
#define BTC_SERIALIZE_BTAGCOMPOUNDENTRY_H

#include <string>

namespace BTC {
namespace serialize_ {

class BTagCompoundEntry
{
public:
    STRING_T tag;
    size_t position;

    bool operator<(const BTagCompoundEntry& entry) const {
        return(tag.compare(entry.tag) < 0);
    }
};

}}

#endif
