#pragma once

#include "tokens.hpp"
#include "traits.hpp"

#include <string>
#include <vector>
#include <map>
#include <tuple>

namespace ecosnail {
namespace config {

template <class T>
struct FieldInfo {
    FieldInfo(std::string name, T &ref) : name(std::move(name)), ref(ref) {}

    std::string name;
    T &ref;
};

/**
 * Information of fields of a struct type.
 *
 * A Fields object stores information on fields of a certain structure or class.
 * It is constructed using list of field names, and list of references to them.
 */
template <class... Types>
class Fields {
public:
    typedef std::tuple<Types...> FieldInfoTuple;
    static constexpr size_t FieldCount = std::tuple_size<FieldInfoTuple>::value;

    Fields(FieldInfoTuple tuple)
        : _fieldInfoTuple(std::move(tuple))
    { }

    template <class T>
    Fields<Types..., FieldInfo<T>> operator()(std::string name, T &ref)
    {
        return Fields<Types..., FieldInfo<T>>(
            std::tuple_cat(
                _fieldInfoTuple,
                std::tuple<FieldInfo<T>>(FieldInfo<T>{ name, ref })
            ));
    }

    constexpr size_t size() const { return FieldCount; }

    // Very, very ineffective. Probably. Replace, if possible.
    // This is actually not so bad, if get<> is O(1) (I think, GCC does
    // something like this).
    template <size_t I = 0>
    void readField(const std::string &fieldName, Tokenizer &tok)
    {
        const auto &field = std::get<I>(_fieldInfoTuple);
        if (field.name == fieldName) {
            read(field.ref, tok);
        } else {
            readField<I+1>(fieldName, tok);
        }
    }

    template <size_t I = 0>
    typename std::enable_if< I < FieldCount >::type
        writeFields(std::ostream &os, int offset)
    {
        const auto &field = std::get<I>(_fieldInfoTuple);
        os << field.name << " = ";
        write(field.ref, os, offset);
        os << std::endl;
        writeFields<I+1>(os, offset);
    }

    template <size_t I>
    typename std::enable_if< I >= FieldCount >::type
        writeFields(std::ostream &, int)
    { }

private:
    FieldInfoTuple _fieldInfoTuple;
};

}} // namespace ecosnail::config
