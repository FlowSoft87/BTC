#ifndef BTC_CONTAINER_ARRAYLIST_H
#define BTC_CONTAINER_ARRAYLIST_H

#include <iostream>
#include <vector>

#include "container_/algorithm.h"

namespace BTC {
namespace container_ {

// Wrapper of the std::vector class.
template<class T>
class ArrayList {

    std::vector<T> data;

public:
    typedef typename std::vector<T>::iterator iterator_type;
    typedef typename std::vector<T>::const_iterator const_iterator_type;

    ArrayList();
    ArrayList(const ArrayList<T>& al);
#ifdef ASSERT_C11
    ArrayList(ArrayList<T>&& al);
#endif
    ArrayList(size_t len);
    ~ArrayList();

#ifdef ASSERT_C11
    template<class... Args> static ArrayList<T> fromValues(const T& val0, const Args&... vals);
    template<class... Args> static ArrayList<T> fromValues(T&& val0, Args&&... vals);
#endif

    ArrayList& operator=(const ArrayList<T>& da);
#ifdef ASSERT_C11
    ArrayList& operator=(ArrayList<T>&& da);
#endif

    // Properties
    size_t size() const;

    // Element access
    const T& operator[](size_t i) const;
    T& operator[](size_t i);
    const T& get(size_t i) const;
    void set(const T& val, size_t i);
#ifdef ASSERT_C11
    void set(T&& val, size_t i);
#endif

    void add(const T& element);
#ifdef ASSERT_C11
    void add(T&& element);
    template<class... Args> void add_new(Args&&... args);
#endif
    template<class Container_T> void add_all(const Container_T& cont);

    void setCapacity(size_t cap);
    void clear();

    // Iterator
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;

    T* getDataPtr();
    const T* getDataPtr() const;
};

template<class T>
ArrayList<T>::ArrayList() : data()
{
}

template<class T>
ArrayList<T>::ArrayList(const ArrayList<T>& al) : data(al.data)
{
}

#ifdef ASSERT_C11
template<class T>
ArrayList<T>::ArrayList(ArrayList<T>&& al) : data(std::move(al.data))
{
}
#endif

template<class T>
ArrayList<T>::ArrayList(size_t len) : data(len)
{
}

template<class T>
ArrayList<T>::~ArrayList()
{
}

#ifdef ASSERT_C11
template<class T>
template<class... Args>
ArrayList<T> ArrayList<T>::fromValues(const T& val0, const Args&... vals)
{
    ArrayList<T> result(sizeof...(Args)+1);
    insert(result,val0,vals...);
    return(result);
}

template<class T>
template<class... Args>
ArrayList<T> ArrayList<T>::fromValues(T&& val0, Args&&... vals)
{
    ArrayList<T> result(sizeof...(Args)+1);
    minsert(result,val0,vals...);
    return(result);
}
#endif

template<class T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& al)
{
    data = al.data;
    return(*this);
}

#ifdef ASSERT_C11
template<class T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& al)
{
    data = std::move(al.data);
    return(*this);
}
#endif

template<class T>
size_t ArrayList<T>::size() const
{
    return(data.size());
}

template<class T>
const T& ArrayList<T>::operator[](size_t i) const
{
#ifdef DEBUG
    if(i > data.size())
    {
        std::cout << "Error (ArrayList.[]): Index out of range!" << std::endl;
        exit(1);
    }
#endif
    return(data.operator[](i));
}

template<class T>
T& ArrayList<T>::operator[](size_t i)
{
#ifdef DEBUG
    if(i > data.size())
    {
        std::cout << "Error (ArrayList.[]): Index out of range!" << std::endl;
        exit(1);
    }
#endif
    return(data.operator[](i));
}

template<class T>
const T& ArrayList<T>::get(size_t i) const
{
#ifdef DEBUG
    if(i > data.size())
    {
        std::cout << "Error (ArrayList.[]): Index out of range!" << std::endl;
        exit(1);
    }
#endif
    return(data.operator[](i));
}

template<class T>
void ArrayList<T>::set(const T& val, size_t i)
{
#ifdef DEBUG
    if(i > data.size())
    {
        std::cout << "Error (ArrayList.[]): Index out of range!" << std::endl;
        exit(1);
    }
#endif
    data[i] = val;
}

#ifdef ASSERT_C11
template<class T>
void ArrayList<T>::set(T&& val, size_t i)
{
#ifdef DEBUG
    if(i > data.size())
    {
        std::cout << "Error (ArrayList.[]): Index out of range!" << std::endl;
        exit(1);
    }
#endif
    data[i] = std::move(val);
}
#endif

template<class T>
void ArrayList<T>::add(const T& element)
{
    data.push_back(element);
}

#ifdef ASSERT_C11
template<class T>
void ArrayList<T>::add(T&& element)
{
    data.push_back(element);
}

template<class T>
template<class... Args>
void ArrayList<T>::add_new(Args&&... args)
{
    data.emplace_back(std::forward<Args>(args)...);
}
#endif

template<class T>
template<class Container_T>
void ArrayList<T>::add_all(const Container_T& cont)
{
    data.insert(data.end(),cont.begin(),cont.end());
}

template<class T>
void ArrayList<T>::setCapacity(size_t cap)
{
    if(cap > data.size()) data.reserve(cap);
    else data.shrink_to_fit();
}

template<class T>
void ArrayList<T>::clear()
{
    data.clear();
}

template<class T>
typename std::vector<T>::iterator ArrayList<T>::begin()
{
    return(data.begin());
}

template<class T>
typename std::vector<T>::iterator ArrayList<T>::end()
{
    return(data.end());
}

template<class T>
typename std::vector<T>::const_iterator ArrayList<T>::begin() const
{
    return(data.begin());
}

template<class T>
typename std::vector<T>::const_iterator ArrayList<T>::end() const
{
    return(data.end());
}

template<class T>
T* ArrayList<T>::getDataPtr()
{
    return(data.data());
}

template<class T>
const T* ArrayList<T>::getDataPtr() const
{
    return(data.data());
}

template<class T>
std::ostream& operator<<(std::ostream& os, const ArrayList<T>& a)
{
    os << "[";
    if(a.size() > 0)
    {
        for(size_t i(0); i<a.size()-1; ++i) os << a.get(i) << ' ';
        os << a.get(a.size()-1) << "]";
    }
    else os << "]";
    return os;
}

}}

#endif
