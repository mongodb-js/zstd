import { expect } from 'chai';

import { compress, decompress } from '../index';

describe('zstd', () => {
  describe('#compress', () => {
    // TODO
    // it('is an async function', () => expect(isAsyncFunction(compress)).to.be.true);

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
    // TODO
    // it('is an async function', () => expect(isAsyncFunction(decompress)).to.be.true);
  });
});
