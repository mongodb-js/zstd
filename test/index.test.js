const { describe, test } = require('mocha');
const { compress, decompress } = require('../index');

const zstdLegacy = require('@mongodb-js/zstd');
const { expect } = require('chai');

describe('compat tests', function () {
  describe('old compress, new decompress', testSuite(zstdLegacy.decompress, compress));
  describe('new compress, decompress', testSuite(decompress, zstdLegacy.compress));
  describe('new compress, new decompress', testSuite(decompress, compress));
});

describe('decompress', function () {
  test('decompress() throws a TypeError', async function () {
    expect(await decompress().catch(e => e))
      .to.be.instanceOf(TypeError)
      .to.match(/must be a buffer/i);
  });

  test('decompress() returns a Nodejs buffer', async function () {
    const compressed = await zstdLegacy.compress(Buffer.from([1, 2, 3]));
    expect(await decompress(compressed)).to.be.instanceOf(Buffer);
  });
});

describe('compress', function () {
  test('compress() throws a TypeError', async function () {
    expect(await compress().catch(e => e))
      .to.be.instanceOf(TypeError)
      .to.match(/must be a buffer/i);
  });

  test('compress() returns a Nodejs buffer', async function () {
    expect(await compress(Buffer.from([1, 2, 3]))).to.be.instanceOf(Buffer);
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
      const input_expected = Array.from({ length: 1_000 }, () => 'a').join('');
      const input = Buffer.from(input_expected, 'utf8');

      const result = Buffer.from(await decompress(await compress(input)));
      expect(result.toString('utf8')).to.deep.equal(input_expected);
    });
  };
}
