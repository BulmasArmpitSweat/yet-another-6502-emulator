CC = gcc
rust_CC = cargo

.PHONY: build clean menuconfig

build:
	@./build.sh build

clean:
	@./build.sh clean

menuconfig:
	@./build.sh menuconfig

test:
	@./build.sh clean
	@./build.sh build
