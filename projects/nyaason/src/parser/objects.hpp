#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace nyaa {

class Object {};

class Nyaa {
public:
    Nyaa();
    Nyaa(const std::type_info& type, Object* ptr);
    Nyaa(Nyaa&& other);

    Nyaa& operator=(Nyaa&& other) noexcept;
    
    template <class T>
    T& get()
    {
        if (typeid(T) != _typeInfo) {
            throw std::runtime_error(
                "Nyaa: wrong type requested: " + typeid(T).name() +
                "; holding: " + _typeInfo.name());
        }

        return *static_cast<T*>(_ptr.get());
    }

    template <class T>
    const T& get() const { return get(); }

private:
    const std::type_info* _typeInfo;
    std::unique_ptr<Object> _ptr;
};

// NOTE: In current implementation, Nyaason objects are primitive wrappers over
// standard library containers. It makes sense to implement real containers
// here.

struct String : public Object {
    std::string value;
};

struct List : public Object {
    std::vector<Nyaa> value;
};

struct Dictionary : public Object {
    std::map<std::string, Nyaa> value;
};

struct Structure : public Object {
    std::string name;
    Dictionary fields;
};


} // namespace nyaa