#ifndef BTC_BTAGCOMPOUND_H
#define BTC_BTAGCOMPOUND_H

#include "container_/ArrayList.h"
#include "container_/algorithm.h"
#include "ptr_/SharedObjPtr.h"

#include "IBTagBase.h"
#include "BTagValueType.h"
#include "BTagArrayType.h"

#include "BTagCompoundEntry.h"
#include "BTagCompoundDataEntry.h"

namespace BTC {

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
    void setTag(const std::string& tag, ptr_::SharedObjPtr<BT> value) {
        if(tag.size() > std::numeric_limits<uint8_t>::max()) {
            std::cout << 
                "Error (serialize_::BTagCompound::addTagCompound): Tag too long!" << 
                std::endl;
            exit(1);
        }
        // Convenience: one would have to actually pass a ptr onto an IBTagBase object.
        // TODO Add a runtime typecheck here! (Flo)
        ptr_::SharedObjPtr<IBTagBase> val = ptr_::SharedObjPtr<IBTagBase>::reinterpretCast(value);
        // Search for tag in tagmap
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
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
    void setValue(const std::string& tag, const T& value) {
        if(tag.size() > std::numeric_limits<uint8_t>::max()) {
            std::cout << 
                "Error (serialize_::BTagCompound::setValue): Tag too long!" << 
                std::endl;
            exit(1);
        }
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr = 
                ptr_::SharedObjPtr<IBTagBase>::fromObject(
                        (IBTagBase*) new BTagValueType<T>(value)
                    );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>::fromObject(
                                    (IBTagBase*) new BTagValueType<T>(value)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is not claimed by this method.
    template<typename T>
    void setArray(const std::string& tag, T* array, size_t len) {
        if(tag.size() > std::numeric_limits<uint8_t>::max()) {
            std::cout << 
                "Error (serialize_::BTagCompound::setArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr = 
                ptr_::SharedObjPtr<IBTagBase>::fromObject(
                        (IBTagBase*) new BTagArrayType<T>(array,len,false)
                    );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>::fromObject(
                                    (IBTagBase*) new BTagArrayType<T>(array,len,false)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Set an entry in the compound that points to the array.
    // Ownership is claimed by this method.
    template<typename T>
    void putArray(const std::string& tag, T* array, size_t len) {
        if(tag.size() > std::numeric_limits<uint8_t>::max()) {
            std::cout << 
                "Error (serialize_::BTagCompound::setArray): Tag too long!" << 
                std::endl;
            exit(1);
        }
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            ptr_::SharedObjPtr<IBTagBase> new_ptr = 
                ptr_::SharedObjPtr<IBTagBase>::fromObject(
                        (IBTagBase*) new BTagArrayType<T>(array,len,true)
                    );
            datalist[tagmap[pos].position].data = new_ptr;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(
                    BTagCompoundDataEntry(
                            tag,
                            ptr_::SharedObjPtr<IBTagBase>::fromObject(
                                    (IBTagBase*) new BTagArrayType<T>(array,len,true)
                                )
                        )
                );
            container_::sort(tagmap);
        }
    }

    // Get methods.
    // BT needs to inherit from IBTagBase.
    template<typename BT>
    const ptr_::SharedObjPtr<BT> getTag(const std::string& tag) const {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            // TODO Add a runtime typecheck here! (Flo)
            return(ptr_::SharedObjPtr<BT>::reinterpretCast(datalist[tagmap[pos].position].data));
        } else {
            std::cout << "Error (serialize_::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    // BT needs to inherit IBTagBase.
    template<typename BT>
    ptr_::SharedObjPtr<BT> getTag(const std::string& tag) {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            // TODO Add a runtime typecheck here! (Flo)
            return(ptr_::SharedObjPtr<BT>::reinterpretCast(datalist[tagmap[pos].position].data));
        } else {
            std::cout << "Error (serialize_::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    //const ptr_::SharedObjPtr<IBTagBase> getTag(size_t index) const;
    //ptr_::SharedObjPtr<IBTagBase> getTag(size_t index); // BTagCompound is mutable.

    template<typename T>
    const T& getValue(const std::string& tag) const {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (datalist[tagmap[pos].position].data->getTypeID() == DataType<T>::value) {
                return((static_cast<BTagValueType<T>&>(*(datalist[tagmap[pos].position].data))).data);
            } else {
                std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
                exit(1);
            }
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    template<typename T>
    T& getValue(const std::string& tag) {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (datalist[tagmap[pos].position].data->getTypeID() == DataType<T>::value) {
                return((static_cast<BTagValueType<T>&>(*(datalist[tagmap[pos].position].data))).data);
            } else {
                std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
                exit(1);
            }
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    // Gets the pointer without claiming ownership.
    template<typename T>
    const T* getArray(const std::string& tag, uint64_t& len) const {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (datalist[tagmap[pos].position].data->getTypeID() == DataType<T*>::value) {
                BTagArrayType<T>& temp = static_cast<BTagArrayType<T>&>(*(datalist[tagmap[pos].position].data));
                len = temp.len;
                return(temp.data);
            } else {
                std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
                exit(1);
            }
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    // Gets the pointer without claiming ownership.
    template<typename T>
    T* getArray(const std::string& tag, uint64_t& len) {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (datalist[tagmap[pos].position].data->getTypeID() == DataType<T*>::value) {
                BTagArrayType<T>& temp = static_cast<BTagArrayType<T>&>(*(datalist[tagmap[pos].position].data));
                len = temp.len;
                return(temp.data);
            } else {
                std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
                exit(1);
            }
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    // Gets the pointer, claims ownership.
    template<typename T>
    T* retrieveArray(const std::string& tag, uint64_t& len) const {
        BTagCompoundEntry searchtag;
        searchtag.tag = tag;
        size_t pos = container_::search_lower(tagmap,searchtag);
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (datalist[tagmap[pos].position].data->getTypeID() == DataType<T*>::value) {
                BTagArrayType<T>& temp = static_cast<BTagArrayType<T>&>(*(datalist[tagmap[pos].position].data));
                temp.owner = false;
                len = temp.len;
                return(temp.data);
            } else {
                std::cout << "Error (BTC::BTagCompound::getValue): Wrong type!" << std::endl;
                exit(1);
            }
        } else {
            std::cout << "Error (BTC::BTagCompound::getTag): Tag does not exist!" << std::endl;
            exit(1);
        }
    }

    size_t size() const {
        return datalist.size();
    }

    void clear() {
        tagmap.clear();
        datalist.clear();
    }

    uint8_t getTypeID() const {
        return DataType<BTagCompound>::value;
    }

    // Serialization methods.
    void serialize(std::ostream& os) const {
        SerializeHelper::serializeIntVar(os,datalist.size());
        for(size_t i=0; i<datalist.size(); ++i) {
            SerializeHelper::serializeString8(os,datalist[i].tag);
            SerializeHelper::serializeByte(os,datalist[i].data->getTypeID());
            datalist[i].data->serialize(os);
        }
    }

    void deserialize(std::istream& is) {
        size_t data_size = SerializeHelper::deserializeIntVar<uint64_t>(is);
        uint8_t type_temp;
        for(size_t i=0; i<data_size; ++i) {
            datalist.add(BTagCompoundDataEntry());
            // tag
            datalist[datalist.size()-1].tag = SerializeHelper::deserializeString8(is);
            // type
            type_temp = SerializeHelper::deserializeByte(is);
            // create new tag
            if(type_temp == DataType<BTagCompound>::value) {
                datalist[datalist.size()-1].data = 
                    ptr_::SharedObjPtr<IBTagBase>::fromObject((IBTagBase*) new BTagCompound());
            } else if(type_temp == DataType<std::string>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagValueType<std::string>()
                        );
            } else if(type_temp == DataType<uint8_t>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagValueType<uint8_t>()
                        );
            } else if(type_temp == DataType<uint16_t>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagValueType<uint16_t>()
                        );
            } else if(type_temp == DataType<uint32_t>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagValueType<uint32_t>()
                        );
            } else if(type_temp == DataType<uint64_t>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagValueType<uint64_t>()
                        );
            } else if(type_temp == DataType<float>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagValueType<float>()
                        );
            } else if((type_temp) == DataType<double>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagValueType<double>()
                        );
            } else if((type_temp) == DataType<uint8_t*>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagArrayType<uint8_t>()
                        );
            } else if((type_temp) == DataType<uint16_t*>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagArrayType<uint16_t>()
                        );
            } else if((type_temp) == DataType<uint32_t*>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagArrayType<uint32_t>()
                        );
            } else if((type_temp) == DataType<uint64_t*>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagArrayType<uint64_t>()
                        );
            } else if((type_temp) == DataType<float*>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagArrayType<float>()
                        );
            } else if((type_temp) == DataType<double*>::value) {
                datalist[datalist.size()-1].data =
                    ptr_::SharedObjPtr<IBTagBase>::fromObject(
                            (IBTagBase*) new BTagArrayType<double>()
                        );
            }
            datalist[datalist.size()-1].data->deserialize(is);
            //std::cout << static_cast<BTagValueType<std::string>*>(data[data.size()-1])->data << std::endl;
            tagmap.add(BTagCompoundEntry());
            tagmap[tagmap.size()-1].tag = datalist[datalist.size()-1].tag;
            tagmap[tagmap.size()-1].position = datalist.size()-1;
        }
        if(tagmap.size() > 1) container_::sort(tagmap);
    }

    std::ostream& print(std::ostream& os, uint16_t increment) const {
        os << int(getTypeID()) << ",{";
        if (datalist.size() == 0) {
            os << "}";
            return os;
        }
        for (size_t i=0, len=datalist.size(); i<len; ++i) {
            os << '\n';
            for (uint16_t i=0; i<(increment+1)*2; ++i) {
                os << ' ';
            }
            os << '(' << i << ",\'" << datalist[i].tag << "\'):";
            datalist[i].data->print(os,increment+1);
        }
        os << '\n';
        for (uint16_t i=0; i<increment*2; ++i) {
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

}

#endif
