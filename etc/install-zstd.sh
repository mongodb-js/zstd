#!/bin/sh
set -o xtrace
set -o errexit

clean_deps() {
	rm -rf deps
}

download_windows() {
	# windows does not support symlinks, so we must download the `win64` build specifically
	curl -L -o zstd.zip "https://github.com/facebook/zstd/releases/download/v$ZSTD_VERSION/zstd-v$ZSTD_VERSION-win64.zip"
	# unlike tar, unzip does not have a "strip components" option.  so we unzip the file, copy all the contents to the correct location,
	# and then delete both the .zip and the unziped content.
	unzip zstd.zip 
	cp -r zstd-v$ZSTD_VERSION-win64/* deps/zstd
	rm zstd.zip
	rm -rf zstd-v$ZSTD_VERSION-win64
}

download_zstd() {
	mkdir -p deps/zstd
	ZSTD_VERSION=$(node -p "require('./package.json')['mongodb:zstd_version']")
	is_windows=$(node -p "process.platform === 'win32'")

	if [ "$is_windows" == "true" ]; then
		download_windows
		exit 0 # no need to build windows
	else
		# -C -> specifies the output location
		# --strip-components -> removes one level of directory nesting
		curl -L "https://github.com/facebook/zstd/releases/download/v$ZSTD_VERSION/zstd-$ZSTD_VERSION.tar.gz" \
	 	| 	tar -zxf - -C deps/zstd --strip-components 1
	fi
}

build_zstd() {
	export MACOSX_DEPLOYMENT_TARGET=11
	cd deps/zstd/build/cmake

	# CMAKE_RC_FLAGS is a workaround for a bug in 1.5.6 that breaks compilation on windows.
	# The fix is merged but not yet released. see https://github.com/facebook/zstd/issues/3999
	cmake -DCMAKE_RC_FLAGS="$(pwd)/lib" -DZSTD_MULTITHREAD_SUPPORT=OFF -DZSTD_BUILD_SHARED=OFF -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' .
	cmake --build .
}

clean_deps
download_zstd
build_zstd