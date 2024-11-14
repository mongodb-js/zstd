const { expect } = require('chai');
const { compress, decompress } = require('../lib/index');

describe('zstd', () => {
  it('compress() works', async function () {
    expect(await compress()).to.deep.equal('compress()');
  });

  it('decompress() works', async function () {
    expect(await decompress()).to.deep.equal('decompress()');
  });

  // actual tests are skipped for now until the C++ compression logic is written.
  describe.skip('#compress', () => {
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

  describe.skip('#decompress', () => {
    context('when decompressing invalid data', () => {
      it('includes a stack trace', async () => {
        try {
          await decompress(Buffer.from('invalid'));
        } catch (error) {
          expect(error.message).to.equal('zstd: Unknown frame descriptor');
          expect(error.stack).to.match(/at decompress/);
        }
      });
    });
  });
});
