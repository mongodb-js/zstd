# @mongodb-js/zstd

[![CI](https://github.com/mongodb-js/zstd/actions/workflows/test.yml/badge.svg)](https://github.com/mongodb-js/zstd/actions/workflows/test.yml)

Zstandard compression library for Node.js

## Installation

```
npm install @mongodb-js/zstd
```

### Release Integrity

Releases are created automatically and signed using the [Node team's GPG key](https://pgp.mongodb.com/node-driver.asc). This applies to the git tag as well as all release packages provided as part of a GitHub release. To verify the provided packages, download the key and import it using gpg:

```
gpg --import node-driver.asc
```

The GitHub release contains a detached signature file for the NPM package (named
`mongodb-js-zstd-X.Y.Z.tgz.sig`).

The following command returns the link npm package. 
```shell
npm view @mongodb-js/zstd@vX.Y.Z dist.tarball 
```

Using the result of the above command, a `curl` command can return the official npm package for the release.

To verify the integrity of the downloaded package, run the following command:
```shell
gpg --verify mongodb-js-zstd-X.Y.Z.tgz.sig mongodb-js-zstd-X.Y.Z.tgz
```

>[!Note]
No verification is done when using npm to install the package. The contents of the Github tarball and npm's tarball are identical.

To verify the native `.node` packages, follow the same steps as above using `mongodb-js-zstd-X.Y.Z-platform.tgz` and the corresponding `.sig` file.


## OS Support matrix

|                  | node12 | node14 | node16 | node18 | node20 |
| ---------------- | ------ | ------ | ------ | ------ | ------ |
| Windows x64      | ✓      | ✓      | ✓      | ✓      | ✓      |
| macOS x64        | ✓      | ✓      | ✓      | ✓      | ✓      |
| macOS arm64      | ✓      | ✓      | ✓      | ✓      | ✓      |
| Linux x64 gnu    | ✓      | ✓      | ✓      | ✓      | ✓      |
| Linux arm gnu    | ✓      | ✓      | ✓      | ✓      | ✓      |
| Linux arm64 gnu  | ✓      | ✓      | ✓      | ✓      | ✓      |
| Linux x64 musl   | ✓      | ✓      | ✓      | ✓      | ✓      |
| Linux arm64 musl | ✓      | ✓      | ✓      | ✓      | ✓      |

## MongoDB Node.js Driver Version Compatibility

Only the following version combinations with the [MongoDB Node.js Driver](https://github.com/mongodb/node-mongodb-native) are considered stable.

|                  | `@mongodb-js/zstd@1.x` |
| ---------------- | ---------------------- |
| `mongodb@>=6.11` | ✓ `^1.1.0` `^2.0.0`    |
| `mongodb@<6.11`  | ✓ `^1.1.0`             |
| `mongodb@5.x`    | ✓                      |
| `mongodb@4.x`    | ✓                      |
| `mongodb@3.x`    | N/A                    |

`@mongodb-js/zstd@2.x` will work with `mongodb@<6.11` but will produce peer dependency errors upon installation.

## API

```ts
export function compress(buffer: Buffer | ArrayBuffer | Uint8Array, level: number): Promise<Buffer>;
export function decompress(buffer: Buffer): Promise<Buffer>;
```

### Bugs / Feature Requests

Think you’ve found a bug? Want to see a new feature in `@mongodb-js/zstd`? Please open a
case in our issue management tool, JIRA:

- Create an account and login [jira.mongodb.org](https://jira.mongodb.org).
- Navigate to the NODE project [jira.mongodb.org/browse/NODE](https://jira.mongodb.org/browse/NODE).
- Click **Create Issue** - Please provide as much information as possible about the issue type and how to reproduce it.

### Support / Feedback

For issues with, questions about, or feedback for the library, please look into our [support channels](https://docs.mongodb.com/manual/support). Please do not email any of the driver developers directly with issues or questions - you're more likely to get an answer on the [MongoDB Community Forums](https://community.mongodb.com/tags/c/drivers-odms-connectors/7/node-js-driver).

### Change Log

Change history can be found in [`HISTORY.md`](https://github.com/mongodb-js/zstd/blob/HEAD/HISTORY.md).

## Usage

```ts
import { compress, decompress } from '@mongodb-js/zstd';

(async () => {
  const buffer = Buffer.from('test');
  const compressed = await compress(buffer, 10);
  const decompressed = await decompress(compressed);
})();
```

## Running Tests

First, install and build the zstd library:

`npm run install-zstd`

Then:
`npm test`

## Releasing

CI will automatically publish when it detects a new release after:

```
npm run release -- --release-as <patch|minor|major>
git push --follow-tags origin main
```
