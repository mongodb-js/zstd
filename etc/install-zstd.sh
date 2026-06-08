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

	# Download to a file before extracting so curl and tar failures are decoupled.
	# Retry loop is used instead of --retry-all-errors because UBI8 ships curl 7.61
	# which predates that flag (added in 7.71). --fail makes curl exit non-zero on
	# HTTP 4xx/5xx (e.g. GitHub rate limiting concurrent CI jobs).
	TMPFILE=$(mktemp)
	ATTEMPTS=0
	until curl -L --fail -o "$TMPFILE" \
		"https://github.com/facebook/zstd/releases/download/v$ZSTD_VERSION/zstd-$ZSTD_VERSION.tar.gz"
	do
		ATTEMPTS=$((ATTEMPTS + 1))
		if [ "$ATTEMPTS" -ge 5 ]; then
			echo "Failed to download zstd after $ATTEMPTS attempts, giving up"
			rm -f "$TMPFILE"
			exit 1
		fi
		echo "Download failed, retrying in 5s (attempt $ATTEMPTS/5)..."
		sleep 5
	done
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