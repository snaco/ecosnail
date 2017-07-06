#include <ecosnail/containers.hpp>

#include <string>
#include <utility>

template <class Key, class Value>
struct HomoValue {
    HomoValue(Key key, Value value)
        : key(std::move(key)), value(std::move(value)) {}

    const Key key;
    const Value value;        
};

template <class Value, class Key, Key... Acceptable>
class HomoPack {
public:
    HomoPack() {}
};

template <class Value, class Key, Key First, Key... Others>
class HomoPack<Value, Key, First, Others...> : HomoPack<Value, Key, Others...> {
public:
    

private:
    
};

enum class Language {
    EN,
    RU,
};

int main()
{
    HomoPack<std::string, Language, Language::EN, Language::RU> pack {
        { Language::EN, "zozozo" },
        { Language::RU, "zuzuzu" },
    };
}