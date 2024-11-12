{
  'targets': [{
    'target_name': 'zstd',
    'type': 'loadable_module',
    'defines': ['ZSTD_STATIC_LINKING_ONLY'],
    'include_dirs': [
        "<!(node -p \"require('node-addon-api').include_dir\")"
    ],
    'variables': {
      'ARCH': '<(host_arch)',
      'libmongocrypt_link_type%': 'static',
      'mongocrypt_avoid_openssl_crypto%': 'false',
      'built_with_electron%': 0
    },
    'sources': [
      'src/addon.cpp'
    ],
    'xcode_settings': {
      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
      'CLANG_CXX_LIBRARY': 'libc++',
      'MACOSX_DEPLOYMENT_TARGET': '10.12',
      'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
    },
    'cflags!': [ '-fno-exceptions' ],
    'cflags_cc!': [ '-fno-exceptions' ],
    'cflags_cc': [  '-std=c++17' ],
    'msvs_settings': {
      'VCCLCompilerTool': { 'ExceptionHandling': 1 },
    }
  }]
}