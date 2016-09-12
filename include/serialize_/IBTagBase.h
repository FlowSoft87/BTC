#ifndef BTC_SERIALIZE_BTAGBASE_H
#define BTC_SERIALIZE_BTAGBASE_H

#include <string>

#include "DataType.h"
#include "function.h"

namespace BTC {
namespace serialize_ {

// Interface for BTag objects.
class IBTagBase {

  public:
    virtual ~IBTagBase() {}
    // Returns the type information.
    virtual unsigned char getTypeID() const = 0;
    // Serialize the BTag to stream.
    virtual void serialize(std::ostream& os) const = 0;
    // Deserialize the BTag from stream.
    virtual void deserialize(std::istream& is) = 0;
    // Human readable print of the BTag
    virtual std::ostream& print(std::ostream& os, unsigned char increment) const = 0;
};

}}

#endif
