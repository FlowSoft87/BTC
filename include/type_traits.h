#ifndef BTC_TYPE_TRAITS_H
#define BTC_TYPE_TRAITS_H

namespace BTC {

template<typename T>
struct is_serializeValue
{
    static const bool value = std::is_arithmetic<T>::value || is_same<std::string,T>::value;
};

template<typename T*>
struct is_serializeArray
{
    static const bool value = std::is_arithmetic<T>::value || is_same<std::string,T>::value;
};

}

#endif
