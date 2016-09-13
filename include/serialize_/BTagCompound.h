#ifndef BTC_BTAGCOMPOUND_H
#define BTC_BTAGCOMPOUND_H

#include "container_/ArrayList.h"
#include "container_/algorithm.h"
#include "ptr_/SharedObjPtr.h"

#include "IBTagBase.h"
#include "BTagByte.h"
#include "BTagShort.h"
#include "BTagInt.h"
#include "BTagLong.h"
#include "BTagFloat.h"
#include "BTagDouble.h"
#include "BTagString.h"
#include "BTagByteArr.h"
#include "BTagShortArr.h"
#include "BTagIntArr.h"
#include "BTagLongArr.h"
#include "BTagFloatArr.h"
#include "BTagDoubleArr.h"
#include "BTagStringArr.h"

#include "BTagCompoundEntry.h"
#include "BTagCompoundDataEntry.h"

namespace BTC {
namespace serialize_ {

/**
 * A binary list that maps tags to data.
 * The order of entries is maintained.
 * Can be serialized to stream.
 */
class BTagCompound : public IBTagBase {

    // Sorted array to find tags using binary search
    container_::ArrayList<BTagCompoundEntry> tagmap;
    container_::ArrayList<BTagCompoundDataEntry> datalist;

public:
    BTagCompound() : tagmap(), datalist() {}

    BTagCompound(const BTagCompound& comp) 
            : tagmap(comp.tagmap), datalist(comp.datalist) {
    }

#ifdef ASSERT_C11
    BTagCompound(BTagCompound&& comp) 
            : tagmap(std::move(comp.tagmap)), datalist(std::move(comp.datalist)) {
    }
#endif

    BTagCompound& operator=(const BTagCompound& comp) {
        datalist = comp.datalist;
        tagmap = comp.tagmap;
        return(*this);
    }

#ifdef ASSERT_C11
    BTagCompound& operator=(BTagCompound&& comp) {
        datalist = std::move(comp.datalist);
        tagmap = std::move(comp.tagmap);
        return(*this);
    }
#endif

    // Add methods

    // Set an IBTagBase object.
    // BT has to derive from IBTagBase.
    template<typename BT>
    void setTag(const STRING_T& tag, ptr_::SharedObjPtr<BT> value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setTag): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        // Convenience: one would have to actually pass a ptr onto an IBTagBase object.
        // TODO Add a runtime typecheck here! (Flo)
        ptr_::SharedObjPtr<IBTagBase> val = ptr_::SharedObjPtr<IBTagBase>::reinterpretCast(value);
        // Search for tag in tagmap
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            datalist[tagmap[pos].position].data = val;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(BTagCompoundDataEntry(searchtag.tag, val));
            container_::sort(tagmap);
        }
    }

    template<typename T>
    void setByte(const STRING_T& tag, T value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setByte): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                        (IBTagBase*) new BTagByte<T>(value)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagByte<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    template<typename T>
    void setShort(const STRING_T& tag, T value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setShort): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                        (IBTagBase*) new BTagShort<T>(value)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagShort<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    template<typename T>
    void setInt(const STRING_T& tag, const T& value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setInt): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                        (IBTagBase*) new BTagInt<T>(value)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagInt<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    template<typename T>
    void setLong(const STRING_T& tag, const T& value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setLong): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                        (IBTagBase*) new BTagLong<T>(value)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagLong<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    template<typename T>
    void setFloat(const STRING_T& tag, const T& value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setFloat): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                        (IBTagBase*) new BTagFloat<T>(value)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagFloat<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    template<typename T>
    void setDouble(const STRING_T& tag, const T& value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setDouble): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                        (IBTagBase*) new BTagDouble<T>(value)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagDouble<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    template<typename T>
    void setString(const STRING_T& tag, const T& value) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setString): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                        (IBTagBase*) new BTagString<T>(value)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagString<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setByteArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setByteArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagByteArr<T>(array,len,false)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagByteArr<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setShortArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setShortArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagShortArr<T>(array,len,false)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagShortArr<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setIntArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagIntArr<T>(array,len,false)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagIntArr<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setLongArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagLongArr<T>(array,len,false)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagLongArr<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setFloatArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagFloatArr<T>(array,len,false)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagFloatArr<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setDoubleArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagDoubleArr<T>(array,len,false)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagDoubleArr<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setStringArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setStringArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagStringArr<T>(array,len,false)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagStringArr<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void passByteArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setByteArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagByteArr<T>(array,len,true)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagByteArr<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void passShortArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setShortArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagShortArr<T>(array,len,true)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagShortArr<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void passIntArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagIntArr<T>(array,len,true)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagIntArr<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void passLongArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagLongArr<T>(array,len,true)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagLongArr<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void passFloatArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagFloatArr<T>(array,len,true)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagFloatArr<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void passDoubleArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setIntArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagDoubleArr<T>(array,len,true)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagDoubleArr<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void passStringArray(const STRING_T& tag, T* array, SIZE_T len) {
#ifdef DEBUG
        if(tag.size() > 256) {
            std::cout << 
                "Error (serialize_::BTagCompound::setStringArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
#endif
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr(
                    (IBTagBase*) new BTagStringArr<T>(array,len,true)
                );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>(
                                    (IBTagBase*) new BTagStringArr<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Get methods.
    // BT needs to inherit from IBTagBase.
    template<typename BT>
    ptr_::SharedConstObjPtr<BT> getTag(const STRING_T& tag) const {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            // TODO Add a runtime typecheck here! (Flo)
            return(ptr_::SharedConstObjPtr<BT>::reinterpretCast(datalist[tagmap[pos].position].data));
        } else {
            std::cout << "Error (serialize_::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    // BT needs to inherit IBTagBase.
    template<typename BT>
    ptr_::SharedObjPtr<BT> getTag(const STRING_T& tag) {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            // TODO Add a runtime typecheck here! (Flo)
            return(ptr_::SharedObjPtr<BT>::reinterpretCast(datalist[tagmap[pos].position].data));
        } else {
            std::cout << "Error (serialize_::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    //const ptr_::SharedObjPtr<IBTagBase> getTag(SIZE_T index) const;
    //ptr_::SharedObjPtr<IBTagBase> getTag(SIZE_T index); // BTagCompound is mutable.

    template<typename T>
    const T& getValue(const STRING_T& tag) const {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            //if (datalist[tagmap[pos].position].data->getTypeID() == DataTypeID::UINT8) {
                return((static_cast<BTagVal<T>&>(*(datalist[tagmap[pos].position].data))).data);
            //} else {
            //    std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
            //    exit(1);
            //}
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    template<typename T>
    T& getValue(const STRING_T& tag) {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            //if (datalist[tagmap[pos].position].data->getTypeID() == DataTypeID::UINT8) {
                return((static_cast<BTagVal<T>&>(*(datalist[tagmap[pos].position].data))).data);
            //} else {
            //    std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
            //    exit(1);
            //}
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    // Gets the pointer without claiming ownership.
    template<typename T>
    const T* getArray(const STRING_T& tag, SIZE_T& len) const {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            //if (datalist[tagmap[pos].position].data->getTypeID() == DataTypeID::value) {
                BTagArr<T>& temp = static_cast<BTagArr<T>&>(*(datalist[tagmap[pos].position].data));
                len = temp.len;
                return(temp.data);
            //} else {
            //    std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
            //    exit(1);
            //}
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    template<typename T>
    T* getArray(const STRING_T& tag, SIZE_T& len) {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
        //    if (datalist[tagmap[pos].position].data->getTypeID() == DataType<T*>::value) {
                BTagArr<T>& temp = static_cast<BTagArr<T>&>(*(datalist[tagmap[pos].position].data));
                len = temp.len;
                return(temp.data);
        //    } else {
        //        std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
        //        exit(1);
        //    }
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    // Gets the pointer, claims ownership.
    template<typename T>
    T* retrieveArray(const STRING_T& tag, SIZE_T& len) {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
        //    if (datalist[tagmap[pos].position].data->getTypeID() == DataType<T*>::value) {
                BTagArr<T>& temp = static_cast<BTagArr<T>&>(*(datalist[tagmap[pos].position].data));
                temp.owner = false;
                len = temp.len;
                return(temp.data);
        //    } else {
        //        std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
        //        exit(1);
        //    }
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    SIZE_T size() const {
        return datalist.size();
    }

    void clear() {
        tagmap.clear();
        datalist.clear();
    }

    UINT8_T getTypeID() const {
        return DataTypeID::COMPOUND;
    }

    // Serialization methods.
    void serialize(std::ostream& os) const {
        serializeIntVar(os,datalist.size());
        for(SIZE_T i=0; i<datalist.size(); ++i) {
            serializeString8(os,datalist[i].tag);
            serializeByte(os,datalist[i].data->getTypeID());
            datalist[i].data->serialize(os);
        }
    }

    void deserialize(std::istream& is) {
        UINT64_T data_size = deserializeIntVar<UINT64_T>(is);
        UINT8_T type_temp;
        for(SIZE_T i=0; i<data_size; ++i) {
            datalist.add(BTagCompoundDataEntry());
            // tag
            datalist[datalist.size()-1].tag = deserializeString8(is);
            // type
            type_temp = deserializeByte(is);
            // create new tag
            if(type_temp == DataTypeID::COMPOUND) {
                datalist[datalist.size()-1].data = 
                    ptr_::SharedObjPtr<IBTagBase>::fromObject((IBTagBase*) new BTagCompound());
            } else if(type_temp == DataTypeID::STRING) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagString<STRING_T>()
                        );
            } else if(type_temp == DataTypeID::UINT8) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagByte<UINT8_T>()
                        );
            } else if(type_temp == DataTypeID::UINT16) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagShort<UINT16_T>()
                        );
            } else if(type_temp == DataTypeID::UINT32) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagInt<UINT32_T>()
                        );
            } else if(type_temp == DataTypeID::UINT64) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagLong<UINT64_T>()
                        );
            } else if(type_temp == DataTypeID::FLOAT) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagFloat<FLOAT_T>()
                        );
            } else if((type_temp) == DataTypeID::DOUBLE) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagDouble<DOUBLE_T>()
                        );
            } else if((type_temp) == DataTypeID::STRING_ARR) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagStringArr<STRING_T>()
                        );
            } else if((type_temp) == DataTypeID::UINT8_ARR) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagByteArr<UINT8_T>()
                        );
            } else if((type_temp) == DataTypeID::UINT16_ARR) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagShortArr<UINT16_T>()
                        );
            } else if((type_temp) == DataTypeID::UINT32_ARR) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagIntArr<UINT32_T>()
                        );
            } else if((type_temp) == DataTypeID::UINT64_ARR) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagLongArr<UINT64_T>()
                        );
            } else if((type_temp) == DataTypeID::FLOAT_ARR) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagFloatArr<FLOAT_T>()
                        );
            } else if((type_temp) == DataTypeID::DOUBLE_ARR) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagDoubleArr<DOUBLE_T>()
                        );
            }
            datalist[datalist.size()-1].data->deserialize(is);
            tagmap.add(BTagCompoundEntry());
            tagmap[tagmap.size()-1].tag = datalist[datalist.size()-1].tag;
            tagmap[tagmap.size()-1].position = datalist.size()-1;
        }
        if(tagmap.size() > 1) container_::sort(tagmap);
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << int(getTypeID()) << ",{";
        if (datalist.size() == 0) {
            os << "}";
            return os;
        }
        for (SIZE_T i=0, len=datalist.size(); i<len; ++i) {
            os << '\n';
            for (UINT8_T j=0; j<(increment+1)*2; ++j) {
                os << ' ';
            }
            os << '(' << i << ",\'" << datalist[i].tag << "\'):";
            datalist[i].data->print(os,increment+1);
        }
        os << '\n';
        for (UINT8_T i=0; i<increment*2; ++i) {
            os << ' ';
        }
        os << "}";
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const BTagCompound& btc);
};

std::ostream& operator<<(std::ostream& os, const BTagCompound& btc) {
    return(btc.print(os,0));
}

}}

#endif
