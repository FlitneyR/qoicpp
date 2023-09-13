#pragma once

namespace QOI
{

typedef uint8_t BYTE;

template<typename T>
T swap_endian(T input)
{
    T output;

    BYTE* source_bytes = reinterpret_cast<BYTE*>(&input);
    BYTE* dest_bytes = reinterpret_cast<BYTE*>(&output);

    for (int i = 0; i < sizeof(T); i++)
        dest_bytes[i] = source_bytes[sizeof(T) - (i + 1)];

    return output;
}

}