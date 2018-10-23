/**
 * Did not bother to write a short comment, so here's a wall of text.
 *
 * This is a huge-ass quick & dirty test for ecosnail::config. It tries to test
 * all at once, maybe not very well.
 *
 * The test is supposed to run fast. There is no big data here (very large
 * configuration or input/output file) -- that should be a subject of another
 * test.
 *
 * Configuration includes different types: fundamental built-in types, standard
 * containers. CV-qualifiers are tested here too, for all types (it probably
 * does not make much sense, but why think when you can just try and forget).
 * Besides, there may be a bug that fails to process const/volatile properly,
 * that is present only for a certain category of types (because different
 * categories follow different code paths), and definition of categories
 * themselves may change.
 *
 * There could be a set of macros to cover all those const-volatile
 * signed/unsigned combinations, but:
 * * There are some minor quirks: signed/unsigned char and char are 3 distinct
 * types. There is no "signed bool" (I think).
 * * You have to remember the order these types appear in (probably).
 * * You still have to write/check them all separately in input/output
 * configuration file, so the pain is there anyway.
 * So, there are no macros.
 *
 * The following reference pages were used:
 * * C++ fundamental types:
 *   http://en.cppreference.com/w/cpp/language/types
 * ...and probably more.
 */

#include <ecosnail/config.hpp>

#include <string>
#include <array>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <forward_list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <typeinfo>

using namespace ecosnail::config;

struct Configuration {

    // Boolean type

    bool                                b;
    const bool                          c_b = true;
    volatile bool                       v_b;
    const volatile bool                 cv_b = true;

    // Character types

    signed char                         sc;
    const signed char                   c_sc = 1;
    volatile signed char                v_sc;
    const volatile signed char          cv_sc = 1;

    unsigned char                       uc;
    const unsigned char                 c_uc = 1;
    volatile unsigned char              v_uc;
    const volatile unsigned char        cv_uc = 1;

    char                                c;
    const char                          c_c = 1;
    volatile char                       v_c;
    const volatile char                 cv_c = 1;

    wchar_t                             wc;
    const wchar_t                       c_wc = 1;
    volatile wchar_t                    v_wc;
    const volatile wchar_t              cv_wc = 1;

    char16_t                            c16;
    const char16_t                      c_c16 = 1;
    volatile char16_t                   v_c16;
    const volatile char16_t             cv_c16 = 1;

    char32_t                            c32;
    const char32_t                      c_c32 = 1;
    volatile char32_t                   v_c32;
    const volatile char32_t             cv_c32 = 1;

    // Integer types

    short                               s;
    const short                         c_s = 1;
    volatile short                      v_s;
    const volatile short                cv_s = 1;

    unsigned short                      us;
    const unsigned short                c_us = 1;
    volatile unsigned short             v_us;
    const volatile unsigned short       cv_us = 1;

    int                                 i;
    const int                           c_i = 1;
    volatile int                        v_i;
    const volatile int                  cv_i = 1;

    unsigned int                        ui;
    const unsigned int                  c_ui = 1;
    volatile unsigned int               v_ui;
    const volatile unsigned int         cv_ui = 1;

    long                                l;
    const long                          c_l = 1;
    volatile long                       v_l;
    const volatile long                 cv_l = 1;

    unsigned long                       ul;
    const unsigned long                 c_ul = 1;
    volatile unsigned long              v_ul;
    const volatile unsigned long        cv_ul = 1;

    long long                           ll;
    const long long                     c_ll = 1;
    volatile long long                  v_ll;
    const volatile long long            cv_ll = 1;

    unsigned long long                  ull;
    const unsigned long long            c_ull = 1;
    volatile unsigned long long         v_ull;
    const volatile unsigned long long   cv_ull = 1;

    // Floating point types
    // TODO: Read special values (infinity, negative zero, NaN, maybe something
    // else).
    
    float                               f;
    const float                         c_f = 1.f;
    volatile float                      v_f;
    const volatile float                cv_f = 1.f;

    double                              d;
    const double                        c_d = 1.;
    volatile double                     v_d;
    const volatile double               cv_d = 1.;

    long double                         ld;
    const long double                   c_ld = 1.;
    volatile long double                v_ld;
    const volatile long double          cv_ld = 1.;

    // String types

    char                                cstr[10];
    std::string                         str;

    // Standard containers
    // NOTE: Probably need to try with different types, too.
    // NOTE: There are also more baroque STL container-like objects, e.g. tuple.

    std::array<int, 10>                 array_int;
    std::vector<int>                    vector_int;
    std::deque<int>                     deque_int;
    std::queue<int>                     queue_int;
    std::stack<int>                     stack_int;
    std::list<int>                      list_int;
    std::forward_list<int>              flist_int;
    std::set<int>                       set_int;
    std::multiset<int>                  mset_int;
    std::unordered_set<int>             uset_int;
    std::unordered_multiset<int>        umset_int;
    std::map<int, int>                  map_int_int;
    std::multimap<int, int>             mmap_int_int;
    std::unordered_map<int, int>        umap_int;
    std::unordered_multimap<int, int>   ummap_int_int;

    decltype(auto) define_fields()
    {
        return Fields<>(std::tuple<>())("f", f)("d", d);
    }
    


    //Fields<FieldInfo<float>> define_fields()
    //{
    //    return std::make_tuple{
    //        { "f", f },
    //    };
    //    //return { FieldInfo<float>("f", f) };
    //    //return Fields<FieldInfo<float>>::FieldInfoTuple (FieldInfo<float>("f", f));
    //}
};

int main()
{
    Configuration conf;
    conf.f = 12.f;


    //std::cout << typeid(Fields<FieldInfo<float>, FieldInfo<double>>::FieldInfoTuple).name() << std::endl;

    ecosnail::config::write(conf, std::cout);
}
