const { expect } = require('chai');
const { compress, decompress } = require('../index');

describe('zstd', () => {
  describe('#compress', () => {
    const buffer = Buffer.from('test');

    it('returns a compressed buffer', async () => {
      const result = await compress(buffer);
      expect(await decompress(result)).to.deep.equal(buffer);
    });
  });
});
