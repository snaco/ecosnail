#include <ecosnail/config.hpp>

#include <vector>
#include <string>
#include <map>
#include <list>
#include <unordered_map>

namespace cfg = ecosnail::config;

struct Configuration {
    std::vector<int> screen_size;
    long bits_per_pixel;
    std::string default_color;

    std::map<std::string, std::map<std::string, std::string>> strings;

    std::list<unsigned> known_prime_numbers;
    std::unordered_map<unsigned, unsigned long long> known_number_squares;
    
    struct ConsoleParameters {
        short width;
        int buffer_height;
        std::vector<std::string> supported_colors;

        DEFINE_FIELDS(width, buffer_height, supported_colors)
    };

    ConsoleParameters console_params;

    DEFINE_FIELDS(
        screen_size, bits_per_pixel, default_color,
        strings,
        known_prime_numbers, known_number_squares,
        console_params)
};

void check(bool condition)
{
    if (!condition) {
        std::cerr << "condition failed" << std::endl;
    }
}


void testRead()
{
    std::string input = R"_(
<
    screen_size = [ 1024 768 ]
    bits_per_pixel = 32
    default_color = "blue"

    strings = {
        "hello" => {
            en => "Good day, say \"hey!\""
            ru => "Добрый день, вам слэш: \\!"
            "fr" => "Bonjour!"
        }

        "bye" => {
            "en" => "Goodbye, good_sir!"
            "ru" => "Досвидули"
            "fr" => "Au Revoir"
        }
    }

    known_prime_numbers = [ 2 3 5 7 11 ]
    known_number_squares = {
        2 => 4
        3 => 9
        5 => 25
        7 => 49
        11 => 121
    }

    console_params = <
        width = 80
        buffer_height = 100
        supported_colors = [ "black" "white" ]
    >
>
    )_";

    std::istringstream stream(input);

    Configuration conf;
    cfg::read(conf, stream);

    check(conf.screen_size == std::vector<int>{ 1024, 768 });
    check(conf.bits_per_pixel == 32);
    check(conf.default_color == "blue");
    
    check(conf.strings == std::map<std::string, std::map<std::string, std::string>> {
        { "hello", {
            { "en", "Good day, say \"hey!\"" },
            { "ru", "Добрый день, вам слэш: \\!" },
            { "fr", "Bonjour!" },
        } },

        { "bye", {
            { "en", "Goodbye, good_sir!" },
            { "ru", "Досвидули" },
            { "fr", "Au Revoir" },
        } },
    });

    check(conf.known_prime_numbers == std::list<unsigned>{ 2, 3, 5, 7, 11 });

    check(conf.known_number_squares == std::unordered_map<unsigned, unsigned long long>{
        { 2, 4 },
        { 3, 9 },
        { 5, 25 },
        { 7, 49 },
        { 11, 121 },
    });

    check(conf.console_params.width == 80);
    check(conf.console_params.buffer_height == 100);
    check(conf.console_params.supported_colors == std::vector<std::string>{"black", "white"});
}

// Check that ucfg can read all fundamental arithmetic types. Overflows are not
// checked. Various inputs are not checked. Just the simple ability to read
// different types.
void testNumericTypesRead()
{
    struct Configuration {
        char char_v;
        unsigned char uchar_v;
        signed char schar_v;

        short short_v;
        unsigned short ushort_v;
        int int_v;
        unsigned int uint_v;
        long long_v;
        unsigned long ulong_v;
        long long llong_v;
        unsigned long long ullong_v;

        float fp;
        double dp;
        long double ldp;

        DEFINE_FIELDS(char_v, uchar_v, schar_v,
            short_v, ushort_v, int_v, uint_v, long_v, ulong_v, llong_v, ullong_v,
            fp, dp, ldp)
    };

    std::string input = R"_(
<
    char_v = 11
    uchar_v = 11
    schar_v = 11

    short_v = 11
    ushort_v = 11
    int_v = 11
    uint_v = 11
    long_v = 11
    ulong_v = 11
    llong_v = 11
    ullong_v = 11

    fp = 11
    dp = 11
    ldp = 11
>
    )_";

    std::istringstream stream(input);
    Configuration conf;
    cfg::read(conf, stream);

    
}