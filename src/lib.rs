#[macro_use]
extern crate napi_derive;
extern crate tokio;

use napi::bindgen_prelude::*;
use async_compression::tokio::bufread::ZstdDecoder;
use async_compression::tokio::write::ZstdEncoder;
use tokio::io::AsyncReadExt as _;
use tokio::io::AsyncWriteExt as _;
use tokio::io::{BufReader, BufWriter};

#[napi]
async fn compress(buffer: Buffer) -> Result<Buffer> {
  let input: Vec<u8> = buffer.into();
  let mut encoder = ZstdEncoder::new(BufWriter::new(input));
  let mut output: Vec<u8> = vec![];
  encoder.write_all(&mut output).await?;
  Ok(Buffer::from(output))
}

#[napi]
async fn decompress(buffer: Buffer) -> Result<Buffer> {
  let input: Vec<u8> = buffer.into();
  let mut decoder = ZstdDecoder::new(BufReader::new(input.as_slice()));
  let mut output: Vec<u8> = vec![];
  decoder.read_to_end(&mut output).await?;
  Ok(Buffer::from(output))
}
