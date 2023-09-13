INCLUDES = -Isrc/headers

LFLAGS = -g -o
CFLAGS = $(INCLUDES) -std=c++2a -g -c -Wall -O2 -o

bin/main: build/main.o build/argsparser.o build/image.o build/qoi_decoder.o build/qoi_encoder.o build/qoi_file.o
	clang++ $(LFLAGS) $@ $^

# Most source files will have associated header files, which they should watch
build/%.o: src/%.cpp src/headers/%.hpp
	clang++ $(CFLAGS) $@ $<

# Some source files may not have header files
build/%.o: src/%.cpp
	clang++ $(CFLAGS) $@ $<

memtest: bin/main
	leaks -atExit -- bin/main

test: bin/main
	bin/main

bench: bin/main
	time bin/main

run: bin/main
	bin/main

clean:
	rm -f build/* bin/*
