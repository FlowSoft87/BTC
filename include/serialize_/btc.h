#ifndef BTC_SERIALIZE_BTC_H
#define BTC_SERIALIZE_BTC_H

#include "container_/ArrayList.h"
#include "container_/algorithm.h"
#include "ptr_/SharedObjPtr.h"

#include "function.h"
#include "data_type.h"
#include "exception.h"

namespace BTC {
namespace serialize_ {


// Logical data types

// Interface for BTag data objects.
class IBTagBase {

  public:
    virtual ~IBTagBase() {}
    // Returns the type information.
    virtual unsigned char getTypeID() const = 0;
    // Get byte-size in stream.
    virtual SIZE_T getByteSize() const = 0;
    // Serialize the BTag to stream.
    virtual void serialize(std::ostream& os) const = 0;
    // Deserialize the BTag from stream.
    virtual void deserialize(std::istream& is) = 0;
    // Human readable print of the BTag
    virtual std::ostream& print(std::ostream& os, unsigned char increment) const = 0;
};

template<typename T>
class BTagVal : public IBTagBase {
    
  public:
    T data;

    BTagVal() : data() {}
    BTagVal(const BTagVal<T>& bt) : data(0) {}
#ifdef ASSERT_C11
    BTagVal(BTagVal<T>&& bt) : data(bt.data) {}
#endif
    BTagVal(const T& value) : data(value) {}
};

template<typename T>
class BTagByte : public BTagVal<T> {

  public:
    BTagByte() : BTagVal<T>() {}
    BTagByte(const BTagByte<T>& bt) : BTagVal<T>(bt) {}
    BTagByte(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT8;
    }

    SIZE_T getByteSize() const {
        return 1;
    }

    void serialize(std::ostream& os) const {
        serializeByte(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeByte(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "b{" << this->data << '}';
        return os;
    }
};

template<typename T>
class BTagShort : public BTagVal<T> {

  public:
    BTagShort() : BTagVal<T>() {}
    BTagShort(const BTagShort<T>& bt) : BTagVal<T>(bt) {}
    BTagShort(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT16;
    }

    SIZE_T getByteSize() const {
        return 2;
    }

    void serialize(std::ostream& os) const {
        serializeShort(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeShort(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "s{" << this->data << '}';
        return os;
    }
};

template<typename T>
class BTagInt : public BTagVal<T> {

  public:
    BTagInt() : BTagVal<T>() {}
    BTagInt(const BTagInt<T>& bt) : BTagVal<T>(bt) {}
    BTagInt(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT32;
    }

    SIZE_T getByteSize() const {
        return 4;
    }

    void serialize(std::ostream& os) const {
        serializeInt(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeInt(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "i{" << this->data << '}';
        return os;
    }
};

template<typename T>
class BTagLong : public BTagVal<T> {

  public:
    BTagLong() : BTagVal<T>() {}
    BTagLong(const BTagLong<T>& bt) : BTagVal<T>(bt) {}
    BTagLong(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::UINT64;
    }

    SIZE_T getByteSize() const {
        return 8;
    }

    void serialize(std::ostream& os) const {
        serializeLong(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeLong(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "l{" << this->data << '}';
        return os;
    }
};

template<typename T>
class BTagFloat : public BTagVal<T> {

  public:
    BTagFloat() : BTagVal<T>() {}
    BTagFloat(const BTagFloat& bt) : BTagVal<T>(bt) {}
    BTagFloat(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::FLOAT;
    }

    SIZE_T getByteSize() const {
        return 4;
    }

    void serialize(std::ostream& os) const {
        serializeFloat(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeFloat(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "f{" << this->data << '}';
        return os;
    }
};

template<typename T>
class BTagDouble : public BTagVal<T> {

  public:
    BTagDouble() : BTagVal<T>() {}
    BTagDouble(const BTagDouble& bt) : BTagVal<T>(bt) {}
    BTagDouble(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::DOUBLE;
    }

    SIZE_T getByteSize() const {
        return 8;
    }

    void serialize(std::ostream& os) const {
        serializeDouble(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeDouble(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "d{" << this->data << '}';
        return os;
    }
};

template<typename T>
class BTagString : public BTagVal<T> {

  public:
    BTagString() : BTagVal<T>() {}
    BTagString(const BTagString& bt) : BTagVal<T>(bt) {}
    BTagString(const T& value) : BTagVal<T>(value) {}

    unsigned char getTypeID() const {
        return DataTypeID::STRING;
    }

    SIZE_T getByteSize() const {
        return getStringByteSize(this->data);
    }

    void serialize(std::ostream& os) const {
        serializeString(os,this->data);
    }

    void deserialize(std::istream& is) {
        this->data = deserializeString(is);
    }

    std::ostream& print(std::ostream& os, unsigned char increment) const {
        os << "st{\"" << this->data << "\"}";
        return os;
    }
};

template<typename T>
class BTagArr : public IBTagBase {
    
  public:
    T* data;
    SIZE_T len;
    bool owner;

    BTagArr() : data(0), len(0), owner(false) {}

    BTagArr(const BTagArr<T>& bt) 
            : data(0), len(bt.len), owner(bt.owner) {
        if(owner) {
            // Copy array
            data = new T[len];
            for(SIZE_T i=0; i<len; ++i) {
                data[i] = bt.data[i];
            }
        } else {
            data = bt.data;
        }
    }

#ifdef ASSERT_C11
    BTagArr(BTagArr<T>&& bt)
            : data(bt.data), len(bt.len), owner(bt.owner) {
        bt.data = 0;
        bt.owner = false;
    }
#endif

    BTagArr(T* value, const SIZE_T& length, bool ownership) 
            : data(value), len(length), owner(ownership) {
    }

    ~BTagArr() {
        if(owner) {
            delete[] data;
        }
    }
};

template<typename T>
class BTagByteArr : public BTagArr<T> {

  public:
    BTagByteArr() : BTagArr<T>() {}
    BTagByteArr(const BTagByteArr<T>& bt) : BTagArr<T>(bt) {}
#ifdef ASSERT_C11
    BTagByteArr(BTagByteArr<T>&& bt) : BTagArr<T>(bt) {}
#endif
    BTagByteArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT8_ARR;
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T bytesize = getIntVarByteSize(this->len);
        // Bytesize of data
        bytesize += this->len;
        return bytesize;
    }

    void serialize(std::ostream& os) const {
        serializeByteArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeByteArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "ba{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

template<typename T>
class BTagShortArr : public BTagArr<T> {

  public:
    BTagShortArr() : BTagArr<T>() {}

    BTagShortArr(const BTagShortArr<T>& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagShortArr(BTagShortArr<T>&& bt) : BTagArr<T>(bt) {}
#endif

    BTagShortArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT16_ARR;
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T bytesize = getIntVarByteSize(this->len);
        // Bytesize of data
        bytesize += this->len*2;
        return bytesize;
    }

    void serialize(std::ostream& os) const {
        serializeShortArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeShortArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "sa{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

template<typename T>
class BTagIntArr : public BTagArr<T> {

  public:
    BTagIntArr() : BTagArr<T>() {}

    BTagIntArr(const BTagIntArr<T>& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagIntArr(BTagIntArr<T>&& bt) : BTagArr<T>(bt) {}
#endif

    BTagIntArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT32_ARR;
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T bytesize = getIntVarByteSize(this->len);
        // Bytesize of data
        bytesize += this->len*4;
        return bytesize;
    }

    void serialize(std::ostream& os) const {
        serializeIntArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeIntArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "ia{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

template<typename T>
class BTagLongArr : public BTagArr<T> {

  public:
    BTagLongArr() : BTagArr<T>() {}

    BTagLongArr(const BTagLongArr<T>& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagLongArr(BTagLongArr<T>&& bt) : BTagArr<T>(bt) {}
#endif

    BTagLongArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::UINT64_ARR;
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T bytesize = getIntVarByteSize(this->len);
        // Bytesize of data
        bytesize += this->len*8;
        return bytesize;
    }

    void serialize(std::ostream& os) const {
        serializeLongArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeLongArray<T>(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "la{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

template<typename T>
class BTagFloatArr : public BTagArr<T> {

  public:
    BTagFloatArr() : BTagArr<T>() {}

    BTagFloatArr(const BTagFloatArr& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagFloatArr(BTagFloatArr&& bt) : BTagArr<T>(bt) {}
#endif

    BTagFloatArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::FLOAT_ARR;
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T bytesize = getIntVarByteSize(this->len);
        // Bytesize of data
        bytesize += this->len*4;
        return bytesize;
    }

    void serialize(std::ostream& os) const {
        serializeFloatArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeFloatArray(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "fa{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

template<typename T>
class BTagDoubleArr : public BTagArr<T> {

  public:
    BTagDoubleArr() : BTagArr<T>() {}

    BTagDoubleArr(const BTagDoubleArr& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagDoubleArr(BTagDoubleArr&& bt) : BTagArr<T>(bt) {}
#endif

    BTagDoubleArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::DOUBLE_ARR;
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T bytesize = getIntVarByteSize(this->len);
        // Bytesize of data
        bytesize += this->len*8;
        return bytesize;
    }

    void serialize(std::ostream& os) const {
        serializeDoubleArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeDoubleArray(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "da{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};

template<typename T>
class BTagStringArr : public BTagArr<T> {

  public:
    BTagStringArr() : BTagArr<T>() {}

    BTagStringArr(const BTagStringArr& bt) : BTagArr<T>(bt) {}

#ifdef ASSERT_C11
    BTagStringArr(BTagStringArr&& bt) : BTagArr<T>(bt) {}
#endif

    BTagStringArr(T* value, const SIZE_T& length, bool ownership) 
            : BTagArr<T>(value,length,ownership) {}

    UINT8_T getTypeID() const {
        return DataTypeID::STRING_ARR;
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T len = this->len;
        SIZE_T bytesize = getIntVarByteSize(len);
        // Bytesize of data
        for (size_t i=0; i<len; ++i) {
            // Bytesize of int var
            bytesize += getStringByteSize(this->data[i]);
        }
        return bytesize;
    }

    void serialize(std::ostream& os) const {
        serializeStringArray(os,this->len,this->data);
    }

    void deserialize(std::istream& is) {
        if(this->owner) {
            delete[] this->data;
        }
        this->data = deserializeStringArray(is,this->len);
        this->owner = true;
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "sta{len=" << this->len << ",own=" << this->owner << '}';
        return os;
    }
};


// BTagCompound

class BTCTagEntry {

  public:
    STRING_T tag;
    size_t position;

    bool operator<(const BTCTagEntry& entry) const {
        return(tag.compare(entry.tag) < 0);
    }
};

class BTCDataEntry {

  public:
    STRING_T tag;
    ptr_::SharedObjPtr<IBTagBase> data;

    BTCDataEntry()
            : tag(), data() {
    }

    BTCDataEntry(const STRING_T& t, ptr_::SharedObjPtr<IBTagBase> d)
            : tag(t), data(d) {
    }
};

// A binary list that maps tags to data.
// The user is responsible for the actual type of his data.
// Setting an entry in this list only specifies how the type
// is serialized.
// Retrieving data with the wrong type specified leads to
// undefined behavior.
// The order of entries is maintained.
// When deserialized from a stream, the data types are set
// to the corresponding type defined by the BTC data types.
// Example:
//     4 Byte integer -> BTC::UINT32_T
//     8 Byte float -> BTC::DOUBLE_T
class BTagCompound : public IBTagBase {

    // Sorted array to find tags using binary search
    container_::ArrayList<BTCTagEntry> tagmap;
    container_::ArrayList<BTCDataEntry> datalist;

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
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists already -> set to new value
            datalist[tagmap[pos].position].data = val;
        } else {
            // Tag does not exist -> add to list
            searchtag.position = datalist.size();
            tagmap.add(searchtag);
            datalist.add(BTCDataEntry(searchtag.tag, val));
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
        setTag(tag, ptr_::SharedObjPtr<BTagByte<T> >(new BTagByte<T>(value)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagShort<T> >(new BTagShort<T>(value)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagInt<T> >(new BTagInt<T>(value)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagLong<T> >(new BTagLong<T>(value)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagFloat<T> >(new BTagFloat<T>(value)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagDouble<T> >(new BTagDouble<T>(value)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagString<T> >(new BTagString<T>(value)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagByteArr<T> >(new BTagByteArr<T>(array,len,false)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagShortArr<T> >(new BTagShortArr<T>(array,len,false)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagIntArr<T> >(new BTagIntArr<T>(array,len,false)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagLongArr<T> >(new BTagLongArr<T>(array,len,false)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagFloatArr<T> >(new BTagFloatArr<T>(array,len,false)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagDoubleArr<T> >(new BTagDoubleArr<T>(array,len,false)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagStringArr<T> >(new BTagStringArr<T>(array,len,false)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagByteArr<T> >(new BTagByteArr<T>(array,len,true)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagShortArr<T> >(new BTagShortArr<T>(array,len,true)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagIntArr<T> >(new BTagIntArr<T>(array,len,true)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagLongArr<T> >(new BTagLongArr<T>(array,len,true)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagFloatArr<T> >(new BTagFloatArr<T>(array,len,true)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagDoubleArr<T> >(new BTagDoubleArr<T>(array,len,true)));
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
        setTag(tag, ptr_::SharedObjPtr<BTagStringArr<T> >(new BTagStringArr<T>(array,len,true)));
    }

    // Get methods.
    // BT needs to inherit from IBTagBase.
    template<typename BT>
    ptr_::SharedConstObjPtr<BT> getTag(const STRING_T& tag) const {
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            // TODO Add a runtime typecheck here! (Flo)
            return(ptr_::SharedConstObjPtr<BT>::reinterpretCast(
                        datalist[tagmap[pos].position].data)
                    );
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::getTag", tag);
        }
    }

    // BT needs to inherit IBTagBase.
    template<typename BT>
    ptr_::SharedObjPtr<BT> getTag(const STRING_T& tag) {
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            // TODO Add a runtime typecheck here! (Flo)
            return(ptr_::SharedObjPtr<BT>::reinterpretCast(datalist[tagmap[pos].position].data));
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::getTag", tag);
        }
    }

    template<typename T>
    const T& getValue(const STRING_T& tag) const {
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (isValue(datalist[tagmap[pos].position].data->getTypeID())) {
                return((static_cast<BTagVal<T>&>(*(datalist[tagmap[pos].position].data))).data);
            } else {
                throw wrong_type_error("BTC::serialize_::BTagCompound::getValue", "value");
            }
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::getValue", tag);
        }
    }

    template<typename T>
    T& getValue(const STRING_T& tag) {
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (isValue(datalist[tagmap[pos].position].data->getTypeID())) {
                return((static_cast<BTagVal<T>&>(*(datalist[tagmap[pos].position].data))).data);
            } else {
                throw wrong_type_error("BTC::serialize_::BTagCompound::getValue", "value");
            }
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::getValue", tag);
        }
    }

    // Gets the pointer without claiming ownership.
    template<typename T>
    const T* getArray(const STRING_T& tag, SIZE_T& len) const {
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (isArray(datalist[tagmap[pos].position].data->getTypeID())) {
                BTagArr<T>& temp = 
                    static_cast<BTagArr<T>&>(*(datalist[tagmap[pos].position].data));
                len = temp.len;
                return(temp.data);
            } else {
                throw wrong_type_error("BTC::serialize_::BTagCompound::getArray", "array");
            }
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::getArray", tag);
        }
    }

    template<typename T>
    T* getArray(const STRING_T& tag, SIZE_T& len) {
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (isArray(datalist[tagmap[pos].position].data->getTypeID())) {
                BTagArr<T>& temp = 
                    static_cast<BTagArr<T>&>(*(datalist[tagmap[pos].position].data));
                len = temp.len;
                return(temp.data);
            } else {
                throw wrong_type_error("BTC::serialize_::BTagCompound::getArray", "array");
            }
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::getArray", tag);
        }
    }

    // Gets the pointer, claims ownership.
    template<typename T>
    T* retrieveArray(const STRING_T& tag, SIZE_T& len) {
        BTCTagEntry searchtag;
        searchtag.tag = tag;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(tag) == 0)) {
            // Tag exists
            if (isArray(datalist[tagmap[pos].position].data->getTypeID())) {
                BTagArr<T>& temp = 
                    static_cast<BTagArr<T>&>(*(datalist[tagmap[pos].position].data));
                temp.owner = false;
                len = temp.len;
                return(temp.data);
            } else {
                throw wrong_type_error("BTC::serialize_::BTagCompound::retrieveArray", "array");
            }
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::retrieveArray", tag);
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

    UINT8_T getTypeID(const STRING_T& key) const {
        BTCTagEntry searchtag;
        searchtag.tag = key;
        SIZE_T pos = 0;
        if (tagmap.size() > 1) {
            pos = container_::search_lower(tagmap,searchtag);
        }
        if ((pos < tagmap.size()) && (tagmap[pos].tag.compare(key) == 0)) {
            return(datalist[tagmap[pos].position].data->getTypeID());
        } else {
            throw tag_not_found_error("BTC::serialize_::BTagCompound::getTypeID", key);
        }
    }

    SIZE_T getByteSize() const {
        // Bytesize of int var
        SIZE_T datalen = datalist.size();
        SIZE_T bytesize = getIntVarByteSize(datalen);
        // Bytesize of data
        for (SIZE_T i=0; i<datalen; ++i) {
            bytesize += 2;
            bytesize += datalist[i].tag.size();
            bytesize += datalist[i].data->getByteSize();
        }
        return bytesize;
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
            datalist.add(BTCDataEntry());
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
            tagmap.add(BTCTagEntry());
            tagmap[tagmap.size()-1].tag = datalist[datalist.size()-1].tag;
            tagmap[tagmap.size()-1].position = datalist.size()-1;
        }
        if(tagmap.size() > 1) container_::sort(tagmap);
    }

    std::ostream& print(std::ostream& os, UINT8_T increment) const {
        os << "c{";
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
