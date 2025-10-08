import { expect } from 'chai';
import { execSync } from 'child_process';

describe('glibc requirements', function () {
  let lddOutput;
  beforeEach(function () {
    if (process.platform !== 'linux') return this.skip();

    try {
      lddOutput = execSync('ldd --version', { encoding: 'utf8' });
    } catch {
      this.skip();
    }

    return;
  });

  it('glibc is 2.28', function () {
    expect(lddOutput).to.contain('2.28');
  });
});
