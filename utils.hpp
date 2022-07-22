#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <typeinfo>

template <typename T>
inline std::string get_name() {
    return typeid(T).name();
}

#endif