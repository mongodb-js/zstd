'use strict';

const { execSync } = require('node:child_process');
const { readFileSync } = require('node:fs');
const { resolve } = require('node:path');

const xtrace = (...args) => {
  console.log(`running: ${args[0]}`);
  return execSync(...args);
};

const zstdRoot = resolve(__dirname, '../../..');
console.log(`zstd package root: ${zstdRoot}`);

const zstdVersion = JSON.parse(
  readFileSync(resolve(zstdRoot, 'package.json'), { encoding: 'utf8' })
).version;
console.log(`zstd Version: ${zstdVersion}`);

xtrace('npm pack --pack-destination test/bundling/webpack', { cwd: zstdRoot });

xtrace(`npm install --no-save mongodb-js-zstd-${zstdVersion}.tgz`);

console.log('zstd installed!');
