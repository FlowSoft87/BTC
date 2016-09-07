#ifndef PTR_SHAREDCONSTOBJPOINTER_H
#define PTR_SHAREDCONSTOBJPOINTER_H

namespace BTC {
namespace ptr_ {

template<typename T>
struct ConstObjPtrData {

    const T* data;
    size_t counter;

    ConstObjPtrData() : data(0), counter(0) {}

    ~ConstObjPtrData() {
        delete data;
    }
};

template<typename T>
class SharedConstObjPtr {

    template<typename U> friend class SharedConstObjPtr;

    ConstObjPtrData<T>* data;

  public:
    SharedConstObjPtr() : data(0) {
        data = new ConstObjPtrData<T>();
        ++(data->counter);
    }

    SharedConstObjPtr(const SharedConstObjPtr<T>& ptr) : data(ptr.data) {
        ++(data->counter);
    }

    // Const pointer can be constructed from non-const pointer.
    // (But not vice versa!)
    SharedConstObjPtr(const SharedObjPtr<T>& ptr) : data(0) {
        data = reinterpret_cast<ConstObjPtrData<T>*>(ptr.data);
        ++(data->counter);
    }

    SharedConstObjPtr(const T* d) : data(0) {
        data = new ConstObjPtrData<T>();
        data->data = d;
        ++(data->counter);
    }

    ~SharedConstObjPtr() {
        --(data->counter);
        if (data->counter == 0) {
            delete data;
        }
    }

    static SharedConstObjPtr<T> fromObject(const T* d) {
        SharedConstObjPtr ptr(d);
        return ptr;
    }

    template<typename U>
    static SharedConstObjPtr<T> reinterpretCast(SharedConstObjPtr<U>& ptr) {
        SharedConstObjPtr<T> new_ptr;
        delete new_ptr.data;
        new_ptr.data = reinterpret_cast<ConstObjPtrData<T>*>(ptr.data);
        ++((new_ptr.data)->counter);
        return new_ptr;
    }

    SharedConstObjPtr<T>& operator=(const SharedConstObjPtr<T>& ptr) {
        --(data->counter);
        if (data->counter == 0) {
            delete data;
        }
        data = ptr.data;
        ++(data->counter);
    }

    const T& operator*() const {
        return *(data->data);
    }

    const T* operator->() const {
        return data->data;
    }

    void swap(SharedConstObjPtr<T>& ptr) {
        ConstObjPtrData<T>* data_ptr = data;
        data = ptr.data;
        ptr.data = data_ptr;
    }

/*    T& operator[](size_t i) {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedConstObjPtr.[]): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data[i];
    }

    const T& operator[](size_t i) const {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedConstObjPtr.[]): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data[i];
    }

    T* operator+(size_t i) {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedConstObjPtr.+): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data+i;
    }

    const T* operator+(size_t i) const {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedConstObjPtr.+): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data+i;
    }*/
};

}}

#endif
