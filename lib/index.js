'use strict';
const { resolve } = require('path');
const { promisify } = require('util');
const { isUint8Array } = require('util/types');

const zstd = require('../build/Release/zstd.node');

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
