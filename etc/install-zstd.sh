#!/bin/sh
set -o xtrace

clean_deps() {
	rm -rf deps
}

download_zstd() {
	rm -rf deps
	mkdir -p deps/zstd
	ZSTD_VERSION=$(node -p "require('./package.json').zstd_version")

	curl -L "https://github.com/facebook/zstd/releases/download/v$ZSTD_VERSION/zstd-$ZSTD_VERSION.tar.gz" \
	 	| tar  -zxf - -C deps/zstd --strip-components 1
}

build_zstd() {
	export MACOSX_DEPLOYMENT_TARGET=11
	cd deps/zstd/build/cmake

	# CMAKE_RC_FLAGS is a workaround for a bug in 1.5.6 that breaks compilation on windows.
	# The fix is merged but not yet released. see https://github.com/facebook/zstd/issues/3999
	cmake -DCMAKE_RC_FLAGS="$(pwd)/lib" -DZSTD_MULTITHREAD_SUPPORT=OFF -DZSTD_BUILD_SHARED=OFF .
	cmake --build .
}

clean_deps
download_zstd
build_zstd