#pragma once

#include <iostream>
#include <string>
#include <array>

#include "image.hpp"

namespace QOI
{

class invalid_qoi_file : public std::exception
{};

enum ChunkType
{
    RGB, RGBA, INDEX, DIFF, LUMA, RUN
};

#pragma pack(push)
#pragma pack(1) // compiler message to pack this struct with no padding
struct Header
{
    char        magic[4];   // should equal "qoif"
    uint32_t    width;      // the width of the image in pixels
    uint32_t    height;     // the height of the image in pixels
    uint8_t     channels;   // 3 = RGB, 4 = RGBA
    uint8_t     colorspace; // 0 = sRGB with linear alpha, 1 = all channels linear

    /// @brief Retrive the magic string as a std::string. 
    std::string magic_str() const;

    /// @brief Convert this Header into the bytes that would appear in a QOI file.
    ///        Must swap endianness of uint32_t members.
    std::array<BYTE, 14UL> into_bytes() const;

    static Header from_bytes(const BYTE& bytes);

    /// @brief Make the QOI image header for an Image.
    static Header for_image(const Image& image);

    bool has_alpha() const;
};
#pragma pack(pop)

std::ostream& operator<<(std::ostream& os, const Header& header);

}
