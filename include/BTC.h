#ifndef BTC_BTC_H_
#define BTC_BTC_H_

#include "ptr_/SharedObjPtr.h"
#include "ptr_/SharedConstObjPtr.h"
#include "serialize_/DataType.h"
#include "serialize_/BTagCompound.h"

namespace BTC {

// Main objects
typedef serialize_::BTagCompound BTagCompound;
typedef ptr_::SharedObjPtr<BTagCompound> BTagCompoundPtr;
typedef ptr_::SharedConstObjPtr<BTagCompound> BTagCompoundConstPtr;

// Data types
typedef serialize_::SIZE_T SIZE_T;
typedef serialize_::UINT8_T UINT8_T;
typedef serialize_::UINT16_T UINT16_T;
typedef serialize_::UINT32_T UINT32_T;
typedef serialize_::UINT64_T UINT64_T;
typedef serialize_::FLOAT_T FLOAT_T;
typedef serialize_::DOUBLE_T DOUBLE_T;

}

#endif
