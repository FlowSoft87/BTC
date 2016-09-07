#ifndef BTC_BTC_H_
#define BTC_BTC_H_

#include "ptr_/SharedObjPtr.h"
#include "ptr_/SharedConstObjPtr.h"
#include "BTagCompound.h"

namespace BTC {

typedef ptr_::SharedObjPtr<BTagCompound> BTagCompoundPtr;
typedef ptr_::SharedConstObjPtr<BTagCompound> BTagCompoundConstPtr;

}

#endif
