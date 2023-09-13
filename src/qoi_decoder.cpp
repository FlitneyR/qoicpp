#include <iostream>
#include <string>
#include <array>

#include <bitset>

#include "headers/argsparser.hpp"
#include "headers/utils.hpp"
#include "headers/image.hpp"
#include "headers/qoi_file.hpp"
#include "headers/qoi_decoder.hpp"

namespace QOI
{

Decoder::Decoder(const std::vector<BYTE>& bytes) :
    bytes(bytes)
{}

Image Decoder::decode() const
{
    if (!is_valid_qoi_file())
        throw invalid_qoi_file();

    Header header = get_header();

    std::vector<Pixel> pixels = decode_pixels();

    bool has_alpha = header.channels == 4;
    bool is_sRGB = header.colorspace == 0;

    if (!has_alpha)
    for (auto& pixel : pixels)
        pixel.alpha = 255;
    
    return Image(pixels, header.width, header.height, has_alpha, is_sRGB);
}

bool Decoder::is_valid_qoi_file() const
{
    if (bytes.size() < 4) return false;

    Header header = get_header();

    return header.magic_str().compare("qoif") == 0;
}

Header Decoder::get_header() const
{
    return Header::from_bytes(*bytes.data());
}

std::vector<Pixel> Decoder::decode_pixels() const
{
    std::array<Pixel, 64> recent_pixels_hash_table {};
    std::vector<Pixel> decoded_pixels;

    Pixel last_pixel(0, 0, 0, 255);

    for (size_t index = sizeof(Header); index < bytes.size() - 8; index++)
    {
        int red, green, blue, alpha;
        int hash_index;
        int dr, dg, db;
        int run_length;
        Pixel next_pixel;

        switch (determine_block_type(bytes[index]))
        {
        case RGB:
            red = bytes[++index];
            green = bytes[++index];
            blue = bytes[++index];
            alpha = last_pixel.alpha;

            decoded_pixels.push_back(Pixel(red, green, blue, alpha));
            break;

        case RGBA:
            red = bytes[++index];
            green = bytes[++index];
            blue = bytes[++index];
            alpha = bytes[++index];

            decoded_pixels.push_back(Pixel(red, green, blue, alpha));
            break;

        case INDEX:
            hash_index = get_index(bytes[index]);

            decoded_pixels.push_back(recent_pixels_hash_table.at(hash_index));
            break;

        case DIFF:
            get_rgb_differences(bytes[index], dr, dg, db);

            next_pixel = last_pixel;
            next_pixel.red += dr;
            next_pixel.green += dg;
            next_pixel.blue += db;

            decoded_pixels.push_back(next_pixel);
            break;

        case LUMA:
            get_luma_differences(bytes[index], bytes[index + 1], dr, dg, db);
            index++;

            next_pixel = last_pixel;
            next_pixel.red += dr;
            next_pixel.green += dg;
            next_pixel.blue += db;

            decoded_pixels.push_back(next_pixel);
            break;

        case RUN:
            run_length = get_run_length(bytes[index]);

            for (int i = 0; i < run_length; i++)
                decoded_pixels.push_back(last_pixel);
            break;
        }

        last_pixel = decoded_pixels.back();

        size_t last_pixel_hash = last_pixel.hash();
        recent_pixels_hash_table.at(last_pixel_hash) = last_pixel;
    }

    return decoded_pixels;
}

ChunkType Decoder::determine_block_type(BYTE byte) const
{
    if (byte == 0b11111110) return RGB;
    if (byte == 0b11111111) return RGBA;

    byte = byte >> 6;
    if (byte == 0b00) return INDEX;
    if (byte == 0b01) return DIFF;
    if (byte == 0b10) return LUMA;
    if (byte == 0b11) return RUN;

    return RUN;
}

size_t Decoder::get_index(BYTE byte) const
{
    return byte & 0b00111111;
}

void Decoder::get_rgb_differences(BYTE byte, int& dr, int& dg, int& db) const
{
    db = ((byte >> 0) & 0b11) - 2;
    dg = ((byte >> 2) & 0b11) - 2;
    dr = ((byte >> 4) & 0b11) - 2;
}

void Decoder::get_luma_differences(BYTE byte1, BYTE byte2, int& dr, int& dg, int& db) const
{
    int diff_green = (byte1 & 0b00111111) - 32;
    int dr_dg = ((byte2 >> 4) & 0b00001111) - 8;
    int db_dg = ((byte2 >> 0) & 0b00001111) - 8;

    dg = diff_green;
    dr = dr_dg + dg;
    db = db_dg + dg;
}

int Decoder::get_run_length(BYTE byte) const
{
    return (byte & 0b00111111) + 1;
}

}
