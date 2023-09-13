#pragma once

#include <string>

#include "image.hpp"
#include "qoi_file.hpp"

namespace QOI
{

class Decoder
{
    std::vector<BYTE> bytes;

public:
    Decoder(const std::vector<BYTE>& bytes);

    /// @brief Decode these bytes into an Image.
    Image decode() const;

    /// @brief Check if these bytes constitute a QOI file. 
    bool is_valid_qoi_file() const;

    /// @brief Parse the first 14 bytes as a QOI file header. 
    Header get_header() const;

private:
    /// @brief Decode these bytes into pixels.
    std::vector<Pixel> decode_pixels() const;

    /// @brief Determine the type of chunk from its first byte.
    ChunkType determine_block_type(BYTE byte) const;

    /// @brief Get the index from an INDEX chunk. 
    size_t get_index(BYTE byte) const;

    /// @brief Get the delta red, delta green, and delta blue from a DIFF chunk.
    void get_rgb_differences(BYTE byte, int& dr, int& dg, int& db) const;

    /// @brief Get the delta green, delta red - delta green, and delta blue - delta green from a LUMA chunk.
    void get_luma_differences(BYTE byte1, BYTE byte2, int& diff_green, int& dr_dg, int& db_dg) const;

    /// @brief Get the run length from a RUN chunk.
    int get_run_length(BYTE byte) const;
};

}
