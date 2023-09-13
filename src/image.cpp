#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "headers/utils.hpp"
#include "headers/image.hpp"
#include "headers/qoi_decoder.hpp"
#include "headers/qoi_encoder.hpp"

namespace QOI
{

Pixel::Pixel() : Pixel(0, 0, 0, 0) {}

Pixel::Pixel(BYTE red, BYTE green, BYTE blue, BYTE alpha) :
    red(red),
    green(green),
    blue(blue),
    alpha(alpha)
{}

size_t Pixel::hash() const
{
    return ((red * 3) + (green * 5) + (blue * 7) + (alpha * 11)) % 64;
}

bool Pixel::operator==(const Pixel& other) const
{
    return red == other.red
        && green == other.green
        && blue == other.blue
        && alpha == other.alpha;
}

std::ostream& operator<<(std::ostream& os, const Pixel& pixel)
{
    return os << "Pixel("
              << "r: " << static_cast<int>(pixel.red) << ", "
              << "g: " << static_cast<int>(pixel.green) << ", "
              << "b: " << static_cast<int>(pixel.blue) << ", "
              << "a: " << static_cast<int>(pixel.alpha) << ")";
}

Image Image::load(const std::string& filepath)
{
    // first try to load it as a qoi image
    std::ifstream infile(filepath, std::ifstream::binary);

    std::istreambuf_iterator<char> file_start(infile);
    std::istreambuf_iterator<char> file_end;

    std::vector<BYTE> filebytes(file_start, file_end);

    Decoder qoi_decoder(filebytes);

    try {
        return qoi_decoder.decode();
    } catch (invalid_qoi_file e)
    { /* ignore that it couldn't be parsed as QOI, try to load it some other way */ }

    // if that failed, try to load it as another file type
    int width, height, n;

    BYTE* data = stbi_load(filepath.c_str(), &width, &height, &n, 4);

    if (data == nullptr)
        throw std::runtime_error("Failed to load image from file: " + filepath);

    bool has_alpha = n == 4;
    bool is_sRGB = true;

    Image image(data, width, height, has_alpha, is_sRGB);

    stbi_image_free(data);

    return image;
}

void Image::save(const std::string& filepath) const
{
    auto file_has_extension = [filepath](const std::string& ext)
    {
        return (filepath.rfind(ext) == filepath.size() - ext.length());
    };
    
    if (file_has_extension(".qoi") || file_has_extension(".QOI"))
    {
        std::vector<BYTE> bytes = Encoder(*this).encode();
        std::ofstream outfile(filepath);
        outfile.write(reinterpret_cast<char*>(bytes.data()), bytes.size());

        return;
    }
    
    if (file_has_extension(".png") || file_has_extension(".PNG"))
        stbi_write_png(filepath.c_str(), width, height, 4, reinterpret_cast<const uint8_t*>(pixels.data()), width * 4);

    if (file_has_extension(".bmp") || file_has_extension(".BMP"))
        stbi_write_bmp(filepath.c_str(), width, height, 4, reinterpret_cast<const uint8_t*>(pixels.data()));
}

Image::Image(BYTE* data, int width, int height, bool has_alpha, bool is_sRGB) :
    width(width),
    height(height),
    has_alpha(has_alpha),
    is_sRGB(is_sRGB)
{
    for (int i = 0; i < width * height; i++)
    {
        BYTE red, green, blue, alpha;

        red = data[4 * i + 0];
        green = data[4 * i + 1];
        blue = data[4 * i + 2];
        alpha = data[4 * i + 3];

        pixels.push_back({ red, green, blue, alpha });
    }
}

Image::Image(std::vector<Pixel> pixels, int width, int height, bool has_alpha, bool is_sRGB) :
    pixels(pixels),
    width(width),
    height(height),
    has_alpha(has_alpha),
    is_sRGB(is_sRGB)
{}

}
