const { expect } = require('chai');
const { compress, decompress } = require('../index');

describe('zstd', () => {
  describe('#compress', () => {
    const buffer = Buffer.from('test');

    context('when not providing a compression level', () => {
      it('returns a compressed buffer', async () => {
        const result = await compress(buffer);
        expect(await decompress(result)).to.deep.equal(buffer);
      });
    });

    context('when providing a compression level', () => {
      context('when the level is valid', () => {
        it('returns a compressed buffer', async () => {
          const result = await compress(buffer, 1);
          expect(await decompress(result)).to.deep.equal(buffer);
        });
      });

      context('when the level is invalid', () => {
        context('when the level is too high', () => {
          it('returns a compressed buffer', async () => {
            const result = await compress(buffer, 100);
            expect(await decompress(result)).to.deep.equal(buffer);
          });
        });

        context('when the level is too low', () => {
          it('returns a compressed buffer', async () => {
            const result = await compress(buffer, -100);
            expect(await decompress(result)).to.deep.equal(buffer);
          });
        });
      });
    });
  });

  describe('#decompress', () => {
    context('when decompressing invalid data', () => {
      it('includes a stack trace', async () => {
        try {
          await decompress(Buffer.from('invalid'));
        } catch (error) {
          expect(error.message).to.equal('zstd: Unknown frame descriptor');
          expect(error.stack).to.match(/module.exports.decompress/);
        }
      });
    });
  });
});
