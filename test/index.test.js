const { test } = require('mocha');
const { compress, decompress } = require('../lib/index');

const { expect } = require('chai');

describe('compat tests', function () {
  let zstdLegacy;
  try {
    zstdLegacy = require('@mongodb-js/zstd');
  } catch {
    // swallow
  }

  const isLinuxs390x = process.platform === 'linux' && process.arch === 's390x';
  if (!isLinuxs390x) {
    describe('new compress, old decompress', testSuite(zstdLegacy.decompress, compress));
    describe('old compress, new decompress', testSuite(decompress, zstdLegacy.compress));
    describe('new compress, new decompress', testSuite(decompress, compress));
  }
});

describe('decompress', function () {
  test('decompress() throws a TypeError', async function () {
    expect(await decompress().catch(e => e))
      .to.be.instanceOf(TypeError)
      .to.match(/must be a uint8array/i);
  });

  test('decompress() returns a Nodejs buffer', async function () {
    const compressed = await compress(Buffer.from([1, 2, 3]));
    expect(await decompress(compressed)).to.be.instanceOf(Buffer);
  });
});

describe('compress', function () {
  test('compress() supports compressionLevel', async function () {
    const buffer = Buffer.from(historyFile());
    const unspecified = await compress(buffer);
    const specified = await compress(buffer, 22);

    expect(specified.length).to.be.lessThan(unspecified.length);
  });
  test('compress() throws a TypeError', async function () {
    expect(await compress().catch(e => e))
      .to.be.instanceOf(TypeError)
      .to.match(/must be a uint8array/i);
  });

  test('compress() returns a Nodejs buffer', async function () {
    expect(await compress(Buffer.from([1, 2, 3]))).to.be.instanceOf(Buffer);
  });

  test('decompress() with empty buffer', async function () {
    expect(await decompress(Buffer.from([]))).to.deep.equal(Buffer.from([]));
  });
});

/**
 * @param {import('../index').decompress} decompress
 * @param {import('../index').compress} compress
 */
function testSuite(decompress, compress) {
  return function () {
    test('empty', async function () {
      const input = Buffer.from('', 'utf8');
      const result = await decompress(await compress(input));
      expect(result.toString('utf8')).to.deep.equal('');
    });

    test('one element', async function () {
      const input = Buffer.from('a', 'utf8');
      const result = Buffer.from(await decompress(await compress(input)));
      expect(result.toString('utf8')).to.deep.equal('a');
    });

    test('typical length string', async function () {
      const input = Buffer.from('hello, world! my name is bailey', 'utf8');
      const result = Buffer.from(await decompress(await compress(input)));
      expect(result.toString('utf8')).to.deep.equal('hello, world! my name is bailey');
    });

    test('huge array', async function () {
      const input_expected = Array.from({ length: 10_000_000 }, () => 'a').join('');
      const input = Buffer.from(input_expected, 'utf8');

      const result = Buffer.from(await decompress(await compress(input)));
      expect(result.toString('utf8')).to.deep.equal(input_expected);
    });
  };
}

/**
 * This function returns a string that, when compressed with compression level 22,
 * compresses smaller than the default level 3.  This is used to ensure that we handle
 * the compression level option.
 */
function historyFile() {
  return `
# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [v2.0.0-alpha.0](https://github.com/mongodb-js/zstd/compare/v1.2.2...v2.0.0-alpha.0) (2024-11-27)

### Features

* **NODE-4569:** add "musl" builds for Alpine Linux ([#12](https://github.com/mongodb-js/zstd/issues/12)) ([b3dedc3](https://github.com/mongodb-js/zstd/commit/b3dedc31a274df1eecb54b9092c8dd270f31c21c))
* **NODE-6539:** add base napi C++ template with standard Node team tooling ([#28](https://github.com/mongodb-js/zstd/issues/28)) ([8c40b08](https://github.com/mongodb-js/zstd/commit/8c40b08782969c87b85d5d1bea0a753db73cc87f))
* **NODE-6540:** Add c++ zstd compression API ([#30](https://github.com/mongodb-js/zstd/issues/30)) ([6673245](https://github.com/mongodb-js/zstd/commit/667324522600abc6f731b54a9d9a7c6e92954bef))
* **NODE-6588:** add ssdlc to zstd ([#43](https://github.com/mongodb-js/zstd/issues/43)) ([016d857](https://github.com/mongodb-js/zstd/commit/016d857ccb76f01ebaf18e01756b571338723a16))

### [1.2.2](https://github.com/mongodb-js/zstd/compare/v1.2.1...v1.2.2) (2024-09-16)


### Bug Fixes

* **NODE-6381:** use targeting for x86_64 darwin ([#26](https://github.com/mongodb-js/zstd/issues/26)) ([f95c9f6](https://github.com/mongodb-js/zstd/commit/f95c9f6b1e836cce7da4e6955181261110e88487))

### [1.2.1](https://github.com/mongodb-js/zstd/compare/v1.2.0...v1.2.1) (2024-09-06)


### Bug Fixes

* **NODE-6348:** Wrap thrown errors in JS Error objects with stacks ([#25](https://github.com/mongodb-js/zstd/issues/25)) ([af62c4f](https://github.com/mongodb-js/zstd/commit/af62c4f5f816386ce605c20641ad30cc74bb77e2))

## [1.2.0](https://github.com/mongodb-js/zstd/compare/v1.1.0...v1.2.0) (2023-08-29)


### Bug Fixes

* **NODE-5177:** update to latest zstd-sys ([#15](https://github.com/mongodb-js/zstd/issues/15)) ([6b6d8ce](https://github.com/mongodb-js/zstd/commit/6b6d8ce098de757c53fd52a09d47aa3e29ed2902))

## [1.1.0](https://github.com/mongodb-js/zstd/compare/v1.0.0...v1.1.0) (2023-01-23)


### Features

* **NODE-4569:** add "musl" builds for Alpine Linux ([#12](https://github.com/mongodb-js/zstd/issues/12)) ([b3dedc3](https://github.com/mongodb-js/zstd/commit/b3dedc31a274df1eecb54b9092c8dd270f31c21c))

## [1.0.0](https://github.com/mongodb-js/zstd/compare/v0.0.7...v1.0.0) (2022-05-18)

### [0.0.7](https://github.com/mongodb-js/zstd/compare/v0.0.6...v0.0.7) (2022-05-18)


### Bug Fixes

* fix macos arm64 build ([#9](https://github.com/mongodb-js/zstd/issues/9)) ([1ebefde](https://github.com/mongodb-js/zstd/commit/1ebefdedb761b34bcc721a934296d3ac9f0e7a1b))

### [0.0.6](https://github.com/mongodb-js/zstd/compare/v0.0.5...v0.0.6) (2022-05-18)


### Features

* add readme ([833aa92](https://github.com/mongodb-js/zstd/commit/833aa92213236ef35c4bd79d4c462751a3b4e634))
* update readme ([49ac44e](https://github.com/mongodb-js/zstd/commit/49ac44efbe9a4ab544958b97ae178b51cac057ff))

### [0.0.5](https://github.com/mongodb-js/zstd/compare/v0.0.4...v0.0.5) (2022-05-09)


### Bug Fixes

* remove 686 win build ([ea36ddc](https://github.com/mongodb-js/zstd/commit/ea36ddc0ce3e6630321e7074a138f2d45dd16f4f))

### [0.0.4](https://github.com/mongodb-js/zstd/compare/v0.0.3...v0.0.4) (2022-05-06)


### Features

* **NODE-1837:** support passing compression level ([2c1a917](https://github.com/mongodb-js/zstd/commit/2c1a9171c689c1fc87428d383ffeb823291f84cf))

### [0.0.3](https://github.com/mongodb-js/zstd/compare/v0.0.2...v0.0.3) (2022-05-06)


### Bug Fixes

* update publish task regex ([b62d9b0](https://github.com/mongodb-js/zstd/commit/b62d9b0e644c85d3443f738d953ae7816d3eba00))

### [0.0.2](https://github.com/mongodb-js/zstd/compare/v0.0.1...v0.0.2) (2022-05-06)


### Bug Fixes

* dont use org in napi name ([9063ea8](https://github.com/mongodb-js/zstd/commit/9063ea8bb7b187aacd876f75b3e74bc0188e7a2b))

### 0.0.1 (2022-05-05)


### Features

* add release script ([c2c1783](https://github.com/mongodb-js/zstd/commit/c2c1783242766c8b65f57838494d1a3c4dc23305))
* add standard version ([f63a9b9](https://github.com/mongodb-js/zstd/commit/f63a9b95ba261004cb2f481ff201fa2e116d3aed))
* initial project setup ([53671a3](https://github.com/mongodb-js/zstd/commit/53671a393326605650d3ae12959796a6c6976472))
* update package.json files ([7823f1b](https://github.com/mongodb-js/zstd/commit/7823f1b3156f4eacd2c235ac660aa9810eee6f84))
* update to idomatic rust ([16e215a](https://github.com/mongodb-js/zstd/commit/16e215a59817fdf94bb62c8620b49b6255bafda0))


### Bug Fixes

* handle status in error ([e29c0ed](https://github.com/mongodb-js/zstd/commit/e29c0ed3b1077987c28bc4daa11c5d6a01c650cf))
* remove debug js ([c454785](https://github.com/mongodb-js/zstd/commit/c454785a6cbfe63ed21ca3942ce0707e0b399d3f))
`;
}
