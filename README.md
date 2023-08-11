# @mongodb-js/zstd

[![CI](https://github.com/mongodb-js/zstd/actions/workflows/CI.yml/badge.svg)](https://github.com/mongodb-js/zstd/actions/workflows/CI.yml)

Zstandard compression library for Node.js

## Installation

```
npm install @mongodb-js/zstd
```

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

|               | `@mongodb-js/zstd@1.x` |
| ------------- | ---------------------- |
| `mongodb@6.x` | ✓ `^1.1.0`           |
| `mongodb@5.x` | ✓                      |
| `mongodb@4.x` | ✓                      |
| `mongodb@3.x` | N/A                    |

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

`npm test`

## Releasing

CI will automatically publish when it detects a new release after:

```
npm run release -- --release-as <patch|minor|major>
git push --follow-tags origin main
```
