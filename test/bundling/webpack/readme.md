# Webpack zstd setup example

In order to use zstd with webpack there are two changes beyond the default config file needed:
- Set `experiments: { topLevelAwait: true }` in the top-level config object
- Set `resolve: { fallback: { crypto: false } }` in the top-level config object
- `npm install --save-dev node-loader` and use it to handle all .node files.

## Testing

To use this bundler test:
- Make changes to bson
- run `npm run build` in the root of the repo to rebuild the zstd src
- in this directory run `npm run install:zstd` to install zstd as if it were from npm
  - We use a `.tgz` install to make sure we're using exactly what will be published to npm
- run `npm run build` to check that webpack can pull in the changes
