#include <iostream>

#include "argsparser.hpp"
#include "image.hpp"

using namespace QOI;

int main(const int argc, const char** const argv)
{
    ArgsParser::configure(argc, argv);

    const std::string* source_file_name = ArgsParser::source_file_name();
    const std::string* destination_file_name = ArgsParser::destination_file_name();

    if (source_file_name == nullptr)
    {
        std::cerr << "You must provide a source file path: -i <source file path>" << std::endl;
        return 1;
    }

    if (destination_file_name == nullptr)
    {
        std::cerr << "You must provide a destination file path: -o <destination file path>" << std::endl;
        return 1;
    }

    Image source_image = Image::load(*source_file_name);
    source_image.save(*destination_file_name);

    return 0;
}
