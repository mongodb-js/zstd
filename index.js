// NB: If you update any type signatures to diverge from bindings itself, make
// sure to update how index.d.ts is generated (napi build --dts ...)

const { compress: _compress, decompress: _decompress } = require('./bindings');

// Error objects created via napi don't have JS stacks; wrap them so .stack is present
// https://github.com/nodejs/node/issues/25318#issuecomment-451068073

exports.compress = async function compress(data) {
  try {
    return await _compress(data);
  } catch (e) {
    throw new Error(`zstd: ${e.message}`);
  }
};
exports.decompress = async function decompress(data) {
  try {
    return await _decompress(data);
  } catch (e) {
    throw new Error(`zstd: ${e.message}`);
  }
};
