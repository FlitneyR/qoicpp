#include <iostream>

#include "headers/qoi_file.hpp"

namespace QOI
{

std::string Header::magic_str() const
{
    return std::string(magic, 4);
}

Header Header::for_image(const Image& image)
{
    Header header;

    header.magic[0] = 'q';
    header.magic[1] = 'o';
    header.magic[2] = 'i';
    header.magic[3] = 'f';
    header.width = image.width;
    header.height = image.height;
    header.channels = image.has_alpha ? 4 : 3;
    header.colorspace = image.is_sRGB ? 0 : 1;

    return header;
}

std::array<BYTE, 14UL> Header::into_bytes() const
{
    Header header = *this;
    header.width = swap_endian(header.width);
    header.height = swap_endian(header.height);

    std::array<BYTE, sizeof(Header)> bytes;
    memcpy(bytes.data(), &header, sizeof(header));
    return bytes;
}

Header Header::from_bytes(const BYTE& bytes)
{
    Header header;

    memcpy(&header, &bytes, 14UL);

    header.width = swap_endian(header.width);
    header.height = swap_endian(header.height);

    return header;
}

bool Header::has_alpha() const
{
    return channels == 4;
}

std::ostream& operator<<(std::ostream& os, const Header& header)
{
    return os << "Header("
              << "magic: \"" << header.magic << "\", "
              << "width: " << header.width << ", "
              << "height: " << header.height << ", "
              << "channels: " << static_cast<int>(header.channels) << ", "
              << "colorspace: " << static_cast<int>(header.colorspace)
              << ")";
}

}
