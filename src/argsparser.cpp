#include <string>
#include <vector>

#include "headers/argsparser.hpp"

namespace QOI
{

std::vector<std::string> ArgsParser::args;

void ArgsParser::configure(const int argc, const char** const argv)
{
    for (size_t i = 0; i < argc; i++)
        args.push_back(std::string(argv[i]));
}

const std::string* ArgsParser::source_file_name()
{
    for (int i = 0; i < args.size(); i++)
    if (is_source_specifier(args[i]))
        return &args[i+1];

    return nullptr;
}

const std::string* ArgsParser::destination_file_name()
{
    for (int i = 0; i < args.size(); i++)
    if (is_destination_specifier(args[i]))
        return &args[i+1];

    return nullptr;
}

constexpr bool ArgsParser::is_source_specifier(const std::string& arg)
{
    return arg == "-i" || arg == "--input";
}

constexpr bool ArgsParser::is_destination_specifier(const std::string& arg)
{
    return arg == "-o" || arg == "--output";
}

}
