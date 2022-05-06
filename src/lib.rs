#[macro_use]
extern crate napi_derive;

use napi::{
  bindgen_prelude::AsyncTask,
  Env, Error, JsBuffer, JsBufferValue, Ref, Result, Status, Task,
};
use zstd::stream::{encode_all, decode_all};

const DEFAULT_LEVEL: i32 = 3;

struct Encoder {
  data: Ref<JsBufferValue>,
  level: i32
}

#[napi]
impl Task for Encoder {
  type Output = Vec<u8>;
  type JsValue = JsBuffer;

  fn compute(&mut self) -> Result<Self::Output> {
    let data: &[u8] = self.data.as_ref();
    encode_all(data, self.level).map_err(|e| Error::new(Status::GenericFailure, format!("{}", e)))
  }

  fn resolve(&mut self, env: Env, output: Self::Output) -> Result<JsBuffer> {
    env.create_buffer_with_data(output).map(|b| b.into_raw())
  }

  fn finally(&mut self, env: Env) -> Result<()> {
    self.data.unref(env)?;
    Ok(())
  }
}

struct Decoder {
  data: Ref<JsBufferValue>
}

#[napi]
impl Task for Decoder {
  type Output = Vec<u8>;
  type JsValue = JsBuffer;

  fn compute(&mut self) -> Result<Self::Output> {
    let data: &[u8] = self.data.as_ref();
    decode_all(data).map_err(|e| Error::new(Status::GenericFailure, format!("{}", e)))
  }

  fn resolve(&mut self, env: Env, output: Self::Output) -> Result<JsBuffer> {
    env.create_buffer_with_data(output).map(|b| b.into_raw())
  }

  fn finally(&mut self, env: Env) -> Result<()> {
    self.data.unref(env)?;
    Ok(())
  }
}

#[napi]
fn compress(data: JsBuffer, level: Option<i32>) -> Result<AsyncTask<Encoder>> {
  let encoder = Encoder {
    data: data.into_ref()?,
    level: level.unwrap_or(DEFAULT_LEVEL)
  };
  Ok(AsyncTask::new(encoder))
}

#[napi]
fn decompress(data: JsBuffer) -> Result<AsyncTask<Decoder>> {
  let decoder = Decoder {
    data: data.into_ref()?,
  };
  Ok(AsyncTask::new(decoder))
}
