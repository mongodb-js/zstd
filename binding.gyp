{
  'targets': [{
    'target_name': 'zstd',
    'type': 'loadable_module',
    'defines': ['ZSTD_STATIC_LINKING_ONLY'],
    'include_dirs': [
        "<!(node -p \"require('node-addon-api').include_dir\")",
        "<(module_root_dir)/deps/zstd/lib",
    ],
    'variables': {
      'ARCH': '<(host_arch)',
      'built_with_electron%': 0
    },
    'sources': [
      'addon/zstd.cpp',
      'addon/compression_worker.h',
      'addon/compressor.h',
      'addon/decompressor.h',
      'addon/napi_utils.h',
    ],
    'xcode_settings': {
      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
      'CLANG_CXX_LIBRARY': 'libc++',
      'MACOSX_DEPLOYMENT_TARGET': '10.12',
      'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
    },
    'cflags!': [ '-fno-exceptions' ],
    'cflags_cc!': [ '-fno-exceptions' ],
    'msvs_settings': {
      'VCCLCompilerTool': { 'ExceptionHandling': 1 },
    },
	 'link_settings': {
      'libraries': [
        '<(module_root_dir)/deps/zstd/build/cmake/lib/libzstd.a',
      ]
    },
  }]
}