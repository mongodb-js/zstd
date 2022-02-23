#[macro_use]
extern crate napi_derive;

use napi::bindgen_prelude::*;

#[napi]
async fn compress(buffer: Buffer) -> Result<Buffer> {
  Ok(buffer)
}

#[napi]
async fn decompress(buffer: Buffer) -> Result<Buffer> {
  Ok(buffer)
}
