#pragma once

#include <vector>

#include "utils.hpp"

namespace QOI
{

#pragma pack(push)
#pragma pack(1) // compiler message to pack this struct with no padding
struct Pixel
{
    BYTE red, green, blue, alpha;

    Pixel();
    Pixel(BYTE red, BYTE green, BYTE blue, BYTE alpha);

    /// @brief Finds the QOI hash table index for this pixel 
    size_t hash() const;

    bool operator==(const Pixel& other) const;
};
#pragma pack(pop)

std::ostream& operator<<(std::ostream& os, const Pixel& pixel);

class Image
{
public:
    std::vector<Pixel> pixels;
    const int width, height;
    const bool has_alpha;
    const bool is_sRGB;

    /// @brief Read an image from disk, specified by a file path.
    static Image load(const std::string& filepath);

    /// @brief Save an image to disk, specified by a file path.
    void save(const std::string& filepath) const;

    Image(std::vector<Pixel> pixels, int width, int height, bool has_alpha, bool is_sRGB);

private:
    /// @brief Parse an array of bytes into an image object.
    /// @param data A pointer to an array of bytes which must be interpretable as Pixels.
    Image(BYTE* data, int width, int height, bool has_alpha, bool is_sRGB);

};

}
