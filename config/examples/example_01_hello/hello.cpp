// ecosnail/config.hpp header includes the declarations required to use
// ecosnail::config.
#include <ecosnail/config.hpp>

#include <vector>
#include <fstream>


// Pick an appropriate namespace alias for the libraries you are going to use.
namespace cfg = ecosnail::config;

// Configuration for a simple console utility.
struct Configuration {
    std::vector<int> screenSize;
    std::string outputColorName;

    // For every structure or class present in your configuration type, you must
    // define the fields you are willing to set from the configuration file. It
    // is required to make a connection between field names in your structures,
    // and the actual objects they are referencing.
    DEFINE_FIELDS(screenSize, outputColorName)
};

int main()
{
    Configuration conf;

    std::ifstream inputFileStream("hello.cfg");
    if (!inputFileStream.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    cfg::read(conf, inputFileStream);

    std::cout << "screenSize:";
    for (int size : conf.screenSize) {
        std::cout << " " << size;
    }
    std::cout << std::endl;

    std::cout << "outputColorName: " << conf.outputColorName << std::endl;

}
