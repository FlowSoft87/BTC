#ifndef PTR_SHAREDOBJPOINTER_H
#define PTR_SHAREDOBJPOINTER_H

namespace BTC {
namespace ptr_ {

template<typename T>
struct ObjPtrData {

    T* data;
    size_t counter;

    ObjPtrData() : data(0), counter(0) {}

    ~ObjPtrData() {
        delete data;
    }
};

template<typename T>
class SharedObjPtr {

    template<typename U> friend class SharedObjPtr;

    ObjPtrData<T>* data;

  public:
    SharedObjPtr() : data(0) {
        data = new ObjPtrData<T>();
        ++(data->counter);
    }

    SharedObjPtr(const SharedObjPtr<T>& ptr) : data(ptr.data) {
        ++(data->counter);
    }

#ifdef ASSERT_C11
    SharedObjPtr(SharedObjPtr<T>&& ptr) : data(ptr.data) {
        ++(data->counter);
    }
#endif

    ~SharedObjPtr() {
        --(data->counter);
        if (data->counter == 0) {
            delete data;
        }
    }

    static SharedObjPtr<T> fromObject(T* d) {
        SharedObjPtr ptr;
        (ptr.data)->data = d;
        //++((ptr.data)->counter);
        return ptr;
    }

    template<typename U>
    static SharedObjPtr<T> reinterpretCast(SharedObjPtr<U>& ptr) {
        SharedObjPtr<T> new_ptr;
        delete new_ptr.data;
        new_ptr.data = reinterpret_cast<ObjPtrData<T>*>(ptr.data);
        ++((new_ptr.data)->counter);
        return new_ptr;
    }

    SharedObjPtr<T>& operator=(const SharedObjPtr<T>& ptr) {
        --(data->counter);
        if (data->counter == 0) {
            delete data;
        }
        data = ptr.data;
        ++(data->counter);
    }

#ifdef ASSERT_C11
    SharedObjPtr<T>& operator=(SharedObjPtr<T>&& ptr) {
        --(data->counter);
        if (data->counter == 0) {
            delete data;
        }
        data = ptr.data;
        ++(data->counter);
    }
#endif

    T& operator*() {
        return *(data->data);
    }

    const T& operator*() const {
        return *(data->data);
    }

    T* operator->() {
        return data->data;
    }

    const T* operator->() const {
        return data->data;
    }

    void swap(SharedObjPtr<T>& ptr) {
        ObjPtrData<T>* data_ptr = data;
        data = ptr.data;
        ptr.data = data_ptr;
    }

/*    T& operator[](size_t i) {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedObjPtr.[]): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data[i];
    }

    const T& operator[](size_t i) const {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedObjPtr.[]): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data[i];
    }

    T* operator+(size_t i) {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedObjPtr.+): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data+i;
    }

    const T* operator+(size_t i) const {
#ifdef DEBUG
        if (!flags[1]) {
            std::cout << "Error (ptr_::SharedObjPtr.+): Not an array!" << std::endl;
            exit(1);
        }
#endif
        return data+i;
    }*/
};

}}

#endif
