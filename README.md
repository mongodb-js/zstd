# @mongodb-js/zstd

Zstandard compression library for Node.js

## Installation

```
npm install @mongodb-js/zstd
```

## Support matrix

|                  | node12 | node14 | node16 | node17 |
| ---------------- | ------ | ------ | ------ | ------ |
| Windows x64      | ✓      | ✓      | ✓      | ✓      |
| Windows arm64    | ✓      | ✓      | ✓      | ✓      |
| macOS x64        | ✓      | ✓      | ✓      | ✓      |
| macOS arm64      | ✓      | ✓      | ✓      | ✓      |
| Linux x64 gnu    | ✓      | ✓      | ✓      | ✓      |
| Linux x64 musl   | ✓      | ✓      | ✓      | ✓      |
| Linux arm gnu    | ✓      | ✓      | ✓      | ✓      |
| Linux arm64 gnu  | ✓      | ✓      | ✓      | ✓      |
| Linux arm64 musl | ✓      | ✓      | ✓      | ✓      |
| Android arm64    | ✓      | ✓      | ✓      | ✓      |
| Android armv7    | ✓      | ✓      | ✓      | ✓      |
| FreeBSD x64      | ✓      | ✓      | ✓      | ✓      |

## API

```ts
export function compress(buffer: Buffer | ArrayBuffer | Uint8Array, level: number): Promise<Buffer>
export function decompress(buffer: Buffer): Promise<Buffer>
```

### Bugs / Feature Requests

Think you’ve found a bug? Want to see a new feature in `@mongodb-js/zstd`? Please open a
case in our issue management tool, JIRA:

- Create an account and login [jira.mongodb.org](https://jira.mongodb.org).
- Navigate to the NODE project [jira.mongodb.org/browse/NODE](https://jira.mongodb.org/browse/NODE).
- Click **Create Issue** - Please provide as much information as possible about the issue type and how to reproduce it.

Bug reports in JIRA for all driver projects (i.e. NODE, PYTHON, CSHARP, JAVA) and the
Core Server (i.e. SERVER) project are **public**.

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