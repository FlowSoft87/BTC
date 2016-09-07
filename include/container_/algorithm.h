#ifndef ALGORITHM_ALGORITHM_H
#define ALGORITHM_ALGORITHM_H

#include <algorithm>

/**
* Collection of general purpose operations to be performed on containers.
**/

namespace BTC {
namespace container_ {

/**
* Copy elements from one container to the other.
* Iterator: F
**/
template<class Container1, class Container2>
void copy(const Container1& c1, Container2& c2) {
    std::copy(c1.begin(),c1.end(),c2.begin());
}

template<class Container1, class Container2>
void copy(const Container1& c1, Container2& c2, size_t pos0, size_t pos1) {
    std::copy(c1.begin()+pos0,c1.begin()+pos1,c2.begin());
}

#ifdef ASSERT_C11
/**
* Move elements from one container to the other.
* Iterator: F
**/
template<class Container1, class Container2>
void move(const Container1& c1, Container2& c2) {
    std::move(c1.begin(),c1.end(),c2.begin());
}
#endif

/**
* Binary sort, order is not necessarily mantained.
* Iterator: RA
**/
template<typename Container>
void sort(Container& c) {
#ifdef DEBUG
    if(c.size() < 1) {
        std::cout << 
            "Error (container_::algorithm_::sort): Container has size zero!" << 
            std::endl;
        exit(1);
    }
#endif
    std::sort(c.begin(),c.end());
}

template<typename Container, typename Compare>
void sort(Container& c, const Compare& comp) {
#ifdef DEBUG
    if(c.size() < 1) {
        std::cout << 
            "Error (container_::algorithm_::sort): Container has size zero!" << 
            std::endl;
        exit(1);
    }
#endif
    std::sort(c.begin(),c.end(),comp);
}

/**
* Binary search, index returned contains first element that is not smaller than val.
* Container must be sorted using the same criterion.
* Iterator: RA
**/
template<typename Container, typename T>
size_t search_lower(const Container& c, 
                    const T& val) {
    typename Container::const_iterator_type iter = std::lower_bound(c.begin(),c.end(),val);
    return(iter-c.begin());
}

template<typename Container, typename T, typename Compare>
size_t search_lower(const Container& c, 
                    const T& val, 
                    const Compare& comp) {
    typename Container::const_iterator_type iter = std::lower_bound(c.begin(),c.end(),val,comp);
    return(iter-c.begin());
}

}}

#endif
