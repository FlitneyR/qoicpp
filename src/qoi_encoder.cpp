#include <vector>
#include <array>

#include "headers/qoi_encoder.hpp"

namespace QOI
{

Encoder::Encoder(Image source) : source(source) {}

std::vector<BYTE> Encoder::encode()
{
    std::array<Pixel, 64> recent_pixels_hash_table {};
    std::vector<BYTE> encoded_bytes;
    Pixel last_pixel {};
    int run_length = 0;
    bool first_pixel = true;

    if (!source.has_alpha)
    for (auto& pixel : source.pixels)
        pixel.alpha = 255;

    // Add header bytes
    Header header = Header::for_image(source);
    std::array<BYTE, 14UL> header_bytes = header.into_bytes();
    encoded_bytes.insert(encoded_bytes.end(), header_bytes.begin(), header_bytes.end());

    /// Add bytes for pixels
    for (const auto& pixel : source.pixels)
    {
        BYTE dr, dg, db, dr_dg, db_dg;
        size_t pixel_hash = pixel.hash();

        if (pixel == last_pixel && run_length < 62 && !first_pixel)
        {
            if (run_length == 0)
                encoded_bytes.push_back(0b11000000);
            else
                encoded_bytes.back()++;
            
            run_length++;

            if (run_length >= 62) run_length = 0;
            goto __QOI_ENCODER_ENCODE_LOOP_END;
        }

        run_length = 0;

        if (recent_pixels_hash_table[pixel_hash] == pixel)
        {
            encoded_bytes.push_back(pixel_hash);
            
            goto __QOI_ENCODER_ENCODE_LOOP_END;
        }

        dr = (pixel.red - last_pixel.red) + 2;
        dg = (pixel.green - last_pixel.green) + 2;
        db = (pixel.blue - last_pixel.blue) + 2;

        if (dr <= 3 && dg <= 3 && db <= 3 && pixel.alpha == last_pixel.alpha)
        {
            encoded_bytes.push_back(0b01000000 | (dr << 4) | (dg << 2) | (db << 0));
            
            goto __QOI_ENCODER_ENCODE_LOOP_END;
        }

        dg = pixel.green - last_pixel.green;
        dr_dg = (dr - 2) - dg;
        db_dg = (db - 2) - dg;

        dg += 32;
        dr_dg += 8;
        db_dg += 8;

        if (dg <= 63 && dr_dg <= 15 && db_dg <= 15 && pixel.alpha == last_pixel.alpha)
        {
            encoded_bytes.push_back(0b10000000 | dg);
            encoded_bytes.push_back((dr_dg << 4) | (db_dg << 0));
            
            goto __QOI_ENCODER_ENCODE_LOOP_END;
        }

        if (pixel.alpha == last_pixel.alpha)
        {
            encoded_bytes.push_back(0b11111110);
            encoded_bytes.push_back(pixel.red);
            encoded_bytes.push_back(pixel.green);
            encoded_bytes.push_back(pixel.blue);
        }
        else
        {
            encoded_bytes.push_back(0b11111111);
            encoded_bytes.push_back(pixel.red);
            encoded_bytes.push_back(pixel.green);
            encoded_bytes.push_back(pixel.blue);
            encoded_bytes.push_back(pixel.alpha);
        }

__QOI_ENCODER_ENCODE_LOOP_END:
        recent_pixels_hash_table[pixel_hash] = pixel;
        last_pixel = pixel;
        first_pixel = false;
    }

    // Add padding
    std::vector<BYTE> padding(8, 0);
    padding.back() = 1;

    encoded_bytes.insert(encoded_bytes.end(), padding.begin(), padding.end());

    return encoded_bytes;
}

}
