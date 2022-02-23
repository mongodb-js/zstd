const { expect } = require('chai');
const { compress, decompress } = require('../index');

describe('zstd', () => {
  describe('#compress', () => {
    const buffer = Buffer.from('test');

    it('returns the buffer', async () => {
      const result = await compress(buffer);
      expect(result).to.deep.equal(buffer);
    });
  });

  describe('#decompress', () => {
    const buffer = Buffer.from('test');

    it('returns the buffer', async () => {
      const result = await decompress(buffer);
      expect(result).to.deep.equal(buffer);
    });
  });
});
