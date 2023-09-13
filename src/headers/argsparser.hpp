#pragma once

#include <string>
#include <vector>

namespace QOI
{

/// @brief Manages parsing commandline arguments.
class ArgsParser
{
    static std::vector<std::string> args;
public:
    ArgsParser() = delete;

    /// @brief Configure the argument parser from the provided arguments
    static void configure(const int argc, const char** const argv);

    /// @return A pointer to the argument that specifies the
    /// source file name, null if no source is specified.
    static const std::string* source_file_name();

    /// @return A pointer to the argument that specifies the
    /// destination file name, null if no destination is specified.
    static const std::string* destination_file_name();

private:

    /// @brief Determines if an argument specifies that the next argument
    /// is the source file name. Accepts '-i' or '--input'.
    static constexpr bool is_source_specifier(const std::string& arg);

    /// @brief Determines if an argument specifies that the next argument
    /// is the destination file name. Accepts '-o' or '--output'.
    static constexpr bool is_destination_specifier(const std::string& arg);
};

}
