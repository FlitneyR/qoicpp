#pragma once

#include "utils.hpp"
#include "image.hpp"
#include "qoi_file.hpp"

namespace QOI
{

class Encoder
{
public:
    Image source;

    Encoder(Image source);

    /// @brief Encode this image into a QOI file's bytes. 
    std::vector<BYTE> encode();

private:
};

}
