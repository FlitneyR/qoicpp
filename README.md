# QOICPP

QOICPP is a command line tool to convert between QOI and common image formats.

Take a look at [the QOI spec](https://qoiformat.org/qoi-specification.pdf), or [this blog post explaining it](https://phoboslab.org/log/2021/11/qoi-fast-lossless-image-compression)

## Using QOICPP

To use this tool, compile it with `make bin/main`, you'll need `make` and `clang++` installed. Then run it with `bin/main --input input_file.whatever --output output_file.whatever`, you can substitute `--input` and `--output` with `-i` and `-o` if you'd like.

The program will determine whether the input file is a QOI file or not regardless of its file extension, and will save it as a QOI file if the provided output filename has the extension `.qoi` or `.QOI`. Otherwise, it will read many common image formats and save many common image formats, courtesy of [stb_image](https://github.com/nothings/stb).
