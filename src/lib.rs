#[macro_use]
extern crate napi_derive;

use napi::{
  bindgen_prelude::AsyncTask,
  Env, JsBuffer, JsBufferValue, Ref, Result, Task,
};

struct Encoder {
  data: Ref<JsBufferValue>
}

#[napi]
impl Task for Encoder {
  type Output = Vec<u8>;
  type JsValue = JsBuffer;

  fn compute(&mut self) -> Result<Self::Output> {
    let data: &[u8] = self.data.as_ref();
    Ok(data.to_vec())
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
    Ok(data.to_vec())
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
fn compress(data: JsBuffer) -> Result<AsyncTask<Encoder>> {
  let encoder = Encoder {
    data: data.into_ref()?,
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
