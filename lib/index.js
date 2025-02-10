'use strict';
const { promisify } = require('util');
const { isUint8Array } = require('util/types');

function load() {
  try {
    return require('../build/Release/zstd.node');
  } catch {
    // Webpack will fail when just returning the require, so we need to wrap
    // in a try/catch and rethrow.
    /* eslint no-useless-catch: 0 */
    try {
      return require('../build/Debug/zstd.node');
    } catch (error) {
      throw error;
    }
  }
}

const zstd = load();

const _compress = promisify(zstd.compress);
const _decompress = promisify(zstd.decompress);
// Error objects created via napi don't have JS stacks; wrap them so .stack is present
// https://github.com/nodejs/node/issues/25318#issuecomment-451068073

exports.compress = async function compress(data, compressionLevel) {
  if (!isUint8Array(data)) {
    throw new TypeError(`parameter 'data' must be a Uint8Array.`);
  }

  if (compressionLevel != null && typeof compressionLevel !== 'number') {
    throw new TypeError(`parameter 'compressionLevel' must be a number.`);
  }

  try {
    return await _compress(data, compressionLevel ?? 3);
  } catch (e) {
    throw new Error(`zstd: ${e.message}`);
  }
};
exports.decompress = async function decompress(data) {
  if (!isUint8Array(data)) {
    throw new TypeError(`parameter 'data' must be a Uint8Array.`);
  }
  try {
    return await _decompress(data);
  } catch (e) {
    throw new Error(`zstd: ${e.message}`);
  }
};
