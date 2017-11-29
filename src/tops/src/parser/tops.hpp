#pragma once

#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <variant>
#include <stdexcept>
#include <ostream>

#ifdef _MSVC_LANG
    #define LANG_VERSION _MSVC_LANG
#else
    #define LANG_VERSION __cplusplus
#endif

#define HAVE_CPP_17 1
#define HAVE_CPP_14 1
#define HAVE_CPP_11 1

//#if LANG_VERSION == 201703L
//    #define HAVE_CPP_17 1
//    #define HAVE_CPP_14 1
//    #define HAVE_CPP_11 1
//#elif LANG_VERSION == 201402L
//    #define HAVE_CPP_17 0
//    #define HAVE_CPP_14 1
//    #define HAVE_CPP_11 1
//#elif LANG_VERSION == 201103L
//    #define HAVE_CPP_17 0
//    #define HAVE_CPP_14 0
//    #define HAVE_CPP_11 1
//#endif

namespace ecosnail {
namespace tops {

template <class T> struct TopsTypeName;

class Tops {
public:
    using String = std::string;
    using List = std::vector<Tops>;
    using Dictionary = std::map<std::string, Tops>;
    using Value = std::variant<std::monostate, String, List, Dictionary>;

    Tops() {}
    Tops(Value value, std::string name = "");

    //
    // Common operations
    //

    // operator=

    Tops& operator=(const   String& str);

    Tops& operator=(const String::value_type* s);
    Tops& operator=(String::value_type ch);

#if HAVE_CPP_11
    Tops& operator=(String&& str);
    Tops& operator=(std::initializer_list<String::value_type> ilist);
#endif

#if HAVE_CPP_17
    Tops& operator=(std::basic_string_view<char, String::traits_type> sv);
#endif

    // std::string::assign

    Tops& assign(String::size_type count, String::value_type ch);
    Tops& assign(const String& str);

#if !HAVE_CPP_14
    basic_string& assign(const basic_string& str,
        size_type pos,
        size_type count);
#endif

#if HAVE_CPP_14
    Tops& assign(
        const String& str,
        String::size_type pos,
        String::size_type count = String::npos);
#endif

#if HAVE_CPP_11
    Tops& assign(String&& str);
    Tops& assign(std::initializer_list<String::value_type> ilist);
#endif

    Tops& assign(const String::value_type* s, String::size_type count);
    Tops& assign(const String::value_type* s);

    template <class InputIt>
    Tops& assign(InputIt first, InputIt last);

#if HAVE_CPP_17
    Tops& assign(
        std::basic_string_view<String::value_type, String::traits_type> sv);

    template < class T >
    Tops& assign(
        const T& t,
        String::size_type pos,
        String::size_type count = String::npos);
#endif

    // std::string::get_allocator

    String::allocator_type get_allocator() const;

    // std::string::at

    String::reference at(String::size_type pos);
    String::const_reference at(String::size_type pos) const;

    // std::string::operator[]

    String::reference operator[](String::size_type pos);
    String::const_reference operator[](String::size_type pos) const;

    // std::string::front

#if HAVE_CPP_11
    String::value_type& front();
    const String::value_type& front() const;
#endif

    // std::string::back

#if HAVE_CPP_11
    String::value_type& back();
    const String::value_type& back() const;
#endif

    // std::string::data

    const String::value_type* data() const;

#if HAVE_CPP_17
    String::value_type* data();
#endif

    // std::string::c_str

    const String::value_type* c_str() const;

    // std::string::operator basic_string_view

#if HAVE_CPP_17
    operator std::basic_string_view<
        String::value_type, String::traits_type>() const noexcept;
#endif

    // std::string::begin, std::string::cbegin

    String::iterator begin();
    String::const_iterator begin() const;

#if HAVE_CPP_11
    String::const_iterator cbegin() const;
#endif

    // end, cend

    String::iterator end();
    String::const_iterator end() const;

#if HAVE_CPP_11
    String::const_iterator cend() const;
#endif

    // rbegin, crbegin

    String::reverse_iterator rbegin();
    String::const_reverse_iterator rbegin() const;

#if HAVE_CPP_11
    String::const_reverse_iterator crbegin() const;
#endif

    // rend, crend

    String::reverse_iterator rend();
    String::const_reverse_iterator rend() const;

#if HAVE_CPP_11
    String::const_reverse_iterator crend() const;
#endif

    // empty

    bool empty() const;

    // size, length

    String::size_type size() const;
    String::size_type length() const;

    // max_size

    String::size_type max_size() const;

    // reserve

    void reserve(String::size_type new_cap = 0);

    // capacity

    String::size_type capacity() const;

    // shrink to fit

#if HAVE_CPP_11
    void shrink_to_fit();
#endif

    // clear

    void clear();

    // insert

    Tops& insert(String::size_type index, String::size_type count, String::value_type ch);
    Tops& insert(String::size_type index, const String::value_type* s);
    Tops& insert(String::size_type index, const String::value_type* s, String::size_type count);
    Tops& insert(String::size_type index, const String& str);

#if !HAVE_CPP_14
    basic_string& insert(size_type index, const basic_string& str,
        size_type index_str, size_type count);
#endif

#if HAVE_CPP_14
    Tops& insert(String::size_type index, const String& str,
        String::size_type index_str, String::size_type count = String::npos);
#endif

#if !HAVE_CPP_11
    iterator insert(iterator pos, CharT ch);
    void insert(iterator pos, size_type count, CharT ch);
#endif

#if HAVE_CPP_11
    String::iterator insert(String::const_iterator pos, String::value_type ch);
    String::iterator insert(String::const_iterator pos, String::size_type count, String::value_type ch);
#endif

#if !HAVE_CPP_11
    template< class InputIt >
    void insert(iterator pos, InputIt first, InputIt last);
#else
    template< class InputIt >
    String::iterator insert(String::const_iterator pos, InputIt first, InputIt last);
#endif

#if HAVE_CPP_11
    String::iterator insert(String::const_iterator pos, std::initializer_list<String::value_type> ilist);
#endif

#if HAVE_CPP_17
    Tops& insert(String::size_type pos, std::basic_string_view<String::value_type, String::traits_type> sv);
#endif

#if HAVE_CPP_17
    template < class T >
    Tops& insert(String::size_type index, const T& t,
        String::size_type index_str, String::size_type count = String::npos);
#endif

    // erase

    Tops& erase(String::size_type index = 0, String::size_type count = String::npos);

#if !HAVE_CPP_11
    String::iterator erase(String::iterator position);
#else
    String::iterator erase(String::const_iterator position);
#endif

#if !HAVE_CPP_11
    String::iterator erase(String::iterator first, String::iterator last);
#else
    String::iterator erase(String::const_iterator first, String::const_iterator last);
#endif

    // push_back

    void push_back(String::value_type ch);

    // pop_back

#if HAVE_CPP_11
    void pop_back();
#endif

    // append

    Tops& append(String::size_type count, String::value_type ch);
    Tops& append(const String& str);

#if !HAVE_CPP_14
    basic_string& append(const basic_string& str,
        size_type pos,
        size_type count);
#else
    Tops& append(const String& str,
        String::size_type pos,
        String::size_type count = String::npos);
#endif

    Tops& append(const String::value_type* s, String::size_type count);
    Tops& append(const String::value_type* s);

    template< class InputIt >
    Tops& append(InputIt first, InputIt last);

#if HAVE_CPP_11
    Tops& append(std::initializer_list<String::value_type> ilist);
#endif

#if HAVE_CPP_17
    Tops& append(std::basic_string_view<String::value_type, String::traits_type> sv);
#endif

    template < class T >
    Tops& append(const T& t, String::size_type pos,
        String::size_type count = String::npos);

    // operator+=

    Tops& operator+=(const String& str);
    Tops& operator+=(String::value_type ch);
    Tops& operator+=(const String::value_type* s);

#if HAVE_CPP_11
    Tops& operator+=(std::initializer_list<CharT> ilist);
#endif

#if HAVE_CPP_17
    Tops& operator+=(std::basic_string_view<CharT, Traits> sv);
#endif

    // compare

    int compare(const basic_string& str) const;
    int compare(size_type pos1, size_type count1,
        const basic_string& str) const;

#if !HAVE_CPP_14
    int compare(size_type pos1, size_type count1,
        const basic_string& str,
        size_type pos2, size_type count2) const;
#else
    int compare(size_type pos1, size_type count1,
        const basic_string& str,
        size_type pos2, size_type count2 = npos) const;
#endif

    int compare(const CharT* s) const;
    int compare(size_type pos1, size_type count1,
        const CharT* s) const;
    int compare(size_type pos1, size_type count1,
        const CharT* s, size_type count2) const;
#if HAVE_CPP_17
    int compare(std::basic_string_view<CharT, Traits> sv) const;
#endif

#if HAVE_CPP_17
    int compare(size_type pos1, size_type count1,
        std::basic_string_view<CharT, Traits> sv) const;
#endif
    template < class T >
    int compare(size_type pos1, size_type count1,
        const T& t,
        size_type pos2, size_type count2 = npos) const;

    // replace

    basic_string& replace( size_type pos, size_type count,
                           const basic_string& str );
    basic_string& replace( const_iterator first, const_iterator last,
                           const basic_string& str );

#if !HAVE_CPP_14
    basic_string& replace( size_type pos, size_type count,
                           const basic_string& str,
                           size_type pos2, size_type count2 );
#else
    basic_string& replace( size_type pos, size_type count,
                           const basic_string& str,
                           size_type pos2, size_type count2 = npos );
#endif

    template< class InputIt >
    basic_string& replace( const_iterator first, const_iterator last,
                           InputIt first2, InputIt last2 );
    basic_string& replace( size_type pos, size_type count,
                           const CharT* cstr, size_type count2 );
    basic_string& replace( const_iterator first, const_iterator last,
                           const CharT* cstr, size_type count2 );
    basic_string& replace( size_type pos, size_type count,
                           const CharT* cstr );
    basic_string& replace( const_iterator first, const_iterator last,
                           const CharT* cstr );
    basic_string& replace( size_type pos, size_type count,
                           size_type count2, CharT ch );
    basic_string& replace( const_iterator first, const_iterator last,
                           size_type count2, CharT ch );
#if HAVE_CPP_11
    basic_string& replace( const_iterator first, const_iterator last, 
                           std::initializer_list<CharT> ilist );
#endif

#if HAVE_CPP_17
    basic_string& replace( size_type pos, size_type count,
                           std::basic_string_view<CharT, Traits> sv );
    basic_string& replace( const_iterator first, const_iterator last,
                           std::basic_string_view<CharT, Traits> sv );
    template < class T >
    basic_string& replace( size_type pos, size_type count, const T& t,
                           size_type pos2, size_type count2 = npos );
#endif

    // substr

    basic_string substr( size_type pos = 0,
                     size_type count = npos ) const;

    // copy

    size_type copy( CharT* dest,
                size_type count,
                size_type pos = 0) const;

    // resize

    void resize( size_type count );
    void resize( size_type count, CharT ch );

    // swap

#if !HAVE_CPP_17
    void swap( basic_string& other );
#else
    void swap( basic_string& other ) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
            std::allocator_traits<Allocator>::is_always_equal::value);
#endif

    // find

    size_type find( const basic_string& str, size_type pos = 0 ) const
    size_type find( const CharT* s, size_type pos, size_type count ) const;
    size_type find( const CharT* s, size_type pos = 0 ) const;
    size_type find( CharT ch, size_type pos = 0 ) const;

#if HAVE_CPP_17
    size_type find( std::basic_string_view<CharT, Traits> sv,
                    size_type pos = 0) const
#endif

    // rfind

    size_type find( const basic_string& str, size_type pos = 0 ) const
    size_type find( const CharT* s, size_type pos, size_type count ) const;
    size_type find( const CharT* s, size_type pos = 0 ) const;
    size_type find( CharT ch, size_type pos = 0 ) const;

#if HAVE_CPP_17
    size_type find( std::basic_string_view<CharT, Traits> sv,
                    size_type pos = 0) const
#endif

    // find_first_of

    size_type find_first_of( const basic_string& str, size_type pos = 0 ) const;
    size_type find_first_of( const CharT* s, size_type pos, size_type count ) const;
    size_type find_first_of( const CharT* s, size_type pos = 0 ) const;
    size_type find_first_of( CharT ch, size_type pos = 0 ) const;

#if HAVE_CPP_17
    size_type find_first_of( std::basic_string_view<CharT, Traits> sv,
                             size_type pos = 0 ) const
#endif

    // find_first_not_of

    size_type find_first_not_of( const basic_string& str, size_type pos = 0 ) const;
    size_type find_first_not_of( const CharT* s, size_type pos, size_type count ) const;
    size_type find_first_not_of( const CharT* s, size_type pos = 0 ) const;
    size_type find_first_not_of( CharT ch, size_type pos = 0 ) const;
#if HAVE_CPP_17
    size_type find_first_not_of( std::basic_string_view<CharT, Traits> sv,
                                 size_type pos = 0) const
#endif

    // find_last_of

    size_type find_last_of( const basic_string& str, size_type pos = npos ) const;
    size_type find_last_of( const CharT* s, size_type pos, size_type count ) const;
    size_type find_last_of( const CharT* s, size_type pos = npos ) const;
    size_type find_last_of( CharT ch, size_type pos = npos ) const;

#if HAVE_CPP_17
    size_type find_last_of( std::basic_string_view<CharT, Traits> sv,
                            size_type pos = npos) const
#endif

    // find_last_not_of

    size_type find_last_not_of( const basic_string& str, size_type pos = npos ) const;
    size_type find_last_not_of( const CharT* s, size_type pos, size_type count ) const;
    size_type find_last_not_of( const CharT* s, size_type pos = npos ) const;
    size_type find_last_not_of( CharT ch, size_type pos = npos ) const;

#if HAVE_CPP_17
    size_type find_last_not_of( std::basic_string_view<CharT, Traits> sv,
                                size_type pos = npos) const
#endif



    friend std::ostream& operator<<(std::ostream& stream, const Tops& tops);

private:
    void prettyPrint(
        std::ostream& stream,
        int offset = 0,
        bool skipFirst = false) const;

    template <class T> T& get()
    {
        if (std::holds_alternative<std::monostate>(_value)) {
            _value.emplace<T>();
        } else if (!std::holds_alternative<T>(_value)) {
            throw std::runtime_error("not a " + TopsTypeName<T>::value);
        }
        return std::get<T>(_value);
    }

    std::string _name;
    Value _value;
};

template <> struct TopsTypeName<Tops::String> { static const char* value; };
template <> struct TopsTypeName<Tops::List> { static const char* value; };
template <> struct TopsTypeName<Tops::Dictionary> { static const char* value; };

Tops parseDocument(const std::string& path);

}} // namespace ecosnail::tops
