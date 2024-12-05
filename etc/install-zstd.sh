#!/bin/sh
set -o xtrace
set -o errexit

clean_deps() {
	rm -rf deps
}

download_zstd() {
	mkdir -p deps/zstd
	ZSTD_VERSION=$(node -p "require('./package.json')['mongodb:zstd_version']")

	# only unpack the source and build files needed to compile the project
	necessary_files="zstd-$ZSTD_VERSION/build zstd-$ZSTD_VERSION/lib zstd-$ZSTD_VERSION/programs"
	
	# flags
	# -L                       follow redirects
	# -C                       output directory
	# -                        tar from stdin
	# --strip-components       ignore the top-level directory when unpacking
	curl -L "https://github.com/facebook/zstd/releases/download/v$ZSTD_VERSION/zstd-$ZSTD_VERSION.tar.gz" \
	 	| tar  -zxf - -C deps/zstd --strip-components 1 $necessary_files
}

build_zstd() {
	export MACOSX_DEPLOYMENT_TARGET=11
	cd deps/zstd

	mkdir out
	cd out

	# CMAKE_RC_FLAGS is a workaround for a bug in 1.5.6 that breaks compilation on windows.
	# The fix is merged but not yet released. see https://github.com/facebook/zstd/issues/3999
	cmake \
		-DCMAKE_RC_FLAGS="$(pwd)/lib" \
		-DZSTD_MULTITHREAD_SUPPORT=OFF \
		-DZSTD_BUILD_SHARED=OFF \
		-DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' \
		-DCMAKE_BUILD_TYPE=Release \
	    ../build/cmake 

	cmake --build .  --target libzstd_static
}

clean_deps
download_zstd
build_zstd