#!/bin/sh
set -o xtrace

clean_deps() {
	rm -rf deps
}

download_zstd() {
	rm -rf deps
	mkdir -p deps/zstd

	curl -L "https://github.com/facebook/zstd/releases/download/v1.5.6/zstd-1.5.6.tar.gz" \
	 	| tar  -zxf - -C deps/zstd --strip-components 1
}

build_zstd() {
	export MACOSX_DEPLOYMENT_TARGET=11
	cd deps/zstd/build/cmake

	cmake .
	make
}

clean_deps
download_zstd
build_zstd