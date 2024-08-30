CC = gcc
rust_CC = cargo

.PHONY: build clean

build:
	./build.sh build

clean:
	./build.sh clean