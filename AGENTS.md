# AGENTS.md

Instructions for AI coding agents working in this repository. This file is the source of truth; tool-specific files (e.g. CLAUDE.md) should only import it.

## Project Overview

`@mongodb-js/zstd` is a Node.js native addon exposing [Zstandard](https://github.com/facebook/zstd) `compress`/`decompress` as async functions. It is used by the MongoDB Node.js driver to compress wire-protocol messages. The C++ addon is built with node-gyp against a vendored zstd static library. On install, `prebuild-install` looks for a *local* cached addon binary (no remote is configured in package.json `binary`), falling back to a source build.

## Commands

- `npm run clean-install` — download zstd into `deps/` and compile the addon (required before testing after a fresh clone).
- `npm run compile` — recompile the addon only.
- `npm test` — run mocha tests (`test/*.js`); requires a compiled addon.
- `npm run check:eslint` — lint JS/TS.
- `npm run check:clang-format` — check C++ formatting; `npm run clang-format` to fix.

## Structure

- `addon/` — C++ addon files. `zstd.cpp` implements the N-API `compress`/`decompress` bindings, `compression.{h,cpp}` defines/implements the wrappers leveraging zstd. `compression_worker.h` holds the 'async' component of the wrapper fns.
- `lib/index.js` — The entrypoint. Loads the compiled `.node` binary and exports promise-based wrappers.
- `index.d.ts` — The public types.
- `deps/` — vendored zstd sources (version pinned by `mongodb:zstd_version` in package.json). Populated by `etc/install-zstd.sh`, untracked.
- `build/` — node-gyp output, holds .node files to be imported in index.js.
- `test/` — mocha tests, plus `test/bundling/webpack` for bundler compatibility.
- `etc/docker.sh` — runs tests in glibc and musl Docker containers.
- `binding.gyp` — node-gyp build config linking the addon against `deps/`.

## Code Conventions

- **Null checks** — loose equality (`== null`), not `=== null`/`=== undefined`.
- **Type imports** — inline: `import { type Foo }`.
- **Formatting** — Prettier: single quotes, 2-space indent, 100-char width, no trailing commas.

## Commit Messages

[Conventional Commits](https://www.conventionalcommits.org/) optionally with a Jira ticket: `<type>(NODE-XXXX): <subject>` — types: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `chore`; breaking changes use `!` (e.g. `feat(NODE-XXXX)!: …`). This rule is mandatory for PR descriptions, because that is what ends up in the history. The individual commits inside a PR do not have to follow this convention, because we squash PR commits.

## Related Repositories

- [mongodb/node-mongodb-native](https://github.com/mongodb/node-mongodb-native) — the MongoDB Node.js driver, the primary consumer of this package.
