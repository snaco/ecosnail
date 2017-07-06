#pragma once

#include "traits.hpp"
#include "errors.hpp"
#include "categories.hpp"

#include <cassert>
#include <ostream>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cctype>

namespace ecosnail {
namespace config {


inline std::string off(int size)
{
    std::ostringstream os;
    for (int i = 0; i < size; i++) {
        os << " ";
    }
    return os.str();
}


// Scalar

template <class T>
enable_if_category_t<T, cat::scalar>
    write(const T &arg, std::ostream &os, int offset = 0)
{
    os << off(offset) << arg;
}

/*
 * Pair
 */

template <class T>
enable_if_category_t<T, cat::pair>
    write(const T &arg, std::ostream &os, int offset = 0)
{
    write(arg.first, os, offset);
    os << " => ";
    write(arg.second, os);
}

template <class T>
enable_if_category_t<T, cat::sequence>
    write(const T &arg, std::ostream &os, int offset = 0)
{
    os << off(offset) << "[";
    for (typename T::const_iterator it = arg.begin(); it != arg.end(); it++) {
        os << " ";
        write(*it, os);
    }
    os << " ]";
}

template <class T>
enable_if_category_t<T, cat::dictionary>
    write(const T &arg, std::ostream &os, int offset = 0)
{
    os << off(offset) << "{" << std::endl;
    for (typename T::const_iterator it = arg.begin(); it != arg.end(); it++) {
        os << off(offset + 4);
        write(*it, os);
        os << std::endl;
    }
    os << off(offset) << "}";
}

template <class T>
enable_if_category_t<T, cat::unknown>
    write(const T &arg, std::ostream &os, int offset = 0)
{
    static_assert(std::is_same<typename category<T>::type, cat::unknown>::value,
		"ucfg: attempted to write an unsupported type");
}

template <class T>
enable_if_category_t<T, cat::structure>
    write(T &arg, std::ostream &os, int offset = 0)
{
    arg.define_fields().writeFields(os, offset);
}


}} // namespace ecosnail::config
