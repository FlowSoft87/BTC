#ifndef BTC_BTAGBASE_H
#define BTC_BTAGBASE_H

#include <string>

#include "DataType.h"
#include "SerializeHelper.h"

namespace BTC {

// Interface for BTag objects.
class IBTagBase {

  public:
    virtual ~IBTagBase() {}
    // Returns the type information.
    virtual uint8_t getTypeID() const = 0;
    // Serialize the BTag to stream.
    virtual void serialize(std::ostream& os) const = 0;
    // Deserialize the BTag from stream.
    virtual void deserialize(std::istream& is) = 0;
    // Human readable print of the BTag
    virtual std::ostream& print(std::ostream& os, uint16_t increment) const = 0;
};

}

#endif
