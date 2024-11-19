#         #   docker buildx create --name builder --bootstrap --use
# docker --debug buildx build --progress=plain --no-cache \
# 	--platform linux/amd64 --output type=local,dest=./prebuilds,platform-split=false \
# 	--build-arg="P=amd64" \
# 	-f ./Dockerfile.musl  \
# 	.

musl() {
docker --debug buildx build --load --progress=plain --no-cache \
            --platform linux/amd64 --output=type=docker \
            --build-arg="PLATFORM=amd64" \
            --build-arg="NODE_VERSION=16.20.1" \
            --build-arg="RUN_TEST=true" \
            -f ./.github/docker/Dockerfile.musl -t musl-zstd-base \
            .
}


glibc() {
    docker buildx build --load --progress=plain \
            --platform linux/amd64 \
            --build-arg="NODE_ARCH=x64" \
            --build-arg="RUN_TEST=true" \
            -f ./.github/docker/Dockerfile.glibc \
            .
}

musl