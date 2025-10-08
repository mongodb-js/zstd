#! /bin/bash

# script to aid in local testing of linux platforms
# requires a running docker instance

# s390x, arm64, amd64 for ubuntu
# amd64 or arm64v8 for alpine
LINUX_ARCH=amd64

NODE_VERSION=24.9.0

SCRIPT_DIR=$(dirname ${BASH_SOURCE:-$0})
PROJECT_DIR=$SCRIPT_DIR/..

build_and_test_musl() {
    docker buildx create --name builder --bootstrap --use

    docker --debug buildx build --load --progress=plain --no-cache \
        --platform linux/$LINUX_ARCH --output=type=docker \
        --build-arg="PLATFORM=$LINUX_ARCH" \
        --build-arg="NODE_VERSION=$NODE_VERSION" \
        --build-arg="RUN_TEST=true" \
        -f ./.github/docker/Dockerfile.musl -t musl-zstd-base \
        .
}

build_and_test_glibc() {
    docker buildx create --name builder --bootstrap --use

    NODE_ARCH=$(node -p 'process.argv[1] === `amd64` && `x64` || process.argv[1]' $LINUX_ARCH)
    docker buildx build --progress=plain --no-cache \
            --platform linux/$LINUX_ARCH \
            --build-arg="NODE_ARCH=$NODE_ARCH" \
            --build-arg="NODE_VERSION=$NODE_VERSION" \
            --build-arg="RUN_TEST=true" \
            -f ./.github/docker/Dockerfile.glibc -t glibc-zstd-base \
            $PROJECT_DIR
}


build_and_test_glibc