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

	TMPFILE=$(mktemp)
	# flags
	# -L                       follow redirects
	# -o                       download to file (decouples curl errors from tar)
	# --fail                   exit non-zero on HTTP 4xx/5xx so --retry applies
	# --retry                  retry up to 5 times (GitHub releases rate-limits concurrent CI jobs)
	# --retry-delay            wait between retries
	# --retry-all-errors       retry on any error, not just connection failures (curl 7.71+)
	# --strip-components       ignore the top-level directory when unpacking
	curl -L \
		--fail \
		--retry 5 \
		--retry-delay 5 \
		--retry-all-errors \
		-o "$TMPFILE" \
		"https://github.com/facebook/zstd/releases/download/v$ZSTD_VERSION/zstd-$ZSTD_VERSION.tar.gz"
	tar -zxf "$TMPFILE" -C deps/zstd --strip-components 1 $necessary_files
	rm -f "$TMPFILE"
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

	cmake --build .  --target libzstd_static --config Release
}

# If a previous build is restored from cache, skip everything.
if [ -d "deps/zstd/out" ]; then
	echo "deps/zstd already built, skipping download and build"
else
	clean_deps
	download_zstd
	build_zstd
fi