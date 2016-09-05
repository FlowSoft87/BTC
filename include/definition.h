#ifndef BTC_DEFINITION_H_
#define BTC_DEFINITION_H_

namespace BTC {

typedef ptr_::SharedObjPtr<BTagCompound> BTagCompoundPtr;

/*class BTagCompoundPtr
        : public ptr_::SharedObjPtr<BTagCompound> {

  public:
    BTagCompoundPtr()
            : ptr_::SharedObjPtr<BTagCompound>(
                      ptr_::SharedObjPtr<BTagCompound>::fromObject(new BTagCompound())
                  ) {}
};*/

}

#endif
