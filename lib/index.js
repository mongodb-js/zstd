'use strict';

const zstd = require('bindings')('zstd');
const { promisify } = require('util');

const _compress = promisify(zstd.compress);
const _decompress = promisify(zstd.decompress);
// Error objects created via napi don't have JS stacks; wrap them so .stack is present
// https://github.com/nodejs/node/issues/25318#issuecomment-451068073

exports.compress = async function compress(data, compressionLevel) {
  if (!Buffer.isBuffer(data)) {
    throw new TypeError(`parameter 'data' must be a Buffer.`);
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
  if (!Buffer.isBuffer(data)) {
    throw new TypeError(`parameter 'data' must be a Buffer.`);
  }
  try {
    return await _decompress(data);
  } catch (e) {
    throw new Error(`zstd: ${e.message}`);
  }
};
