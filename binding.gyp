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
      'addon/compression.h',
      'addon/compression.cpp'
    ],
    'conditions': [
        [
          'OS=="win"',
          {
            'link_settings': {
              'libraries': [
                '<(module_root_dir)/deps/zstd/out/lib/Release/zstd_static.lib'
              ]
            },
          },
          { # macos and linux
          'link_settings': {
            'libraries': [
              '<(module_root_dir)/deps/zstd/out/lib/libzstd.a',
              ]
            },
          }
        ],
        ['OS=="mac"', {
          'xcode_settings': {
            "OTHER_CFLAGS": [
              "-arch x86_64",
              "-arch arm64"
            ],
            "OTHER_LDFLAGS": [
              "-arch x86_64",
              "-arch arm64"
            ]
          }
        }
        ]
      ],
    'cflags!': [ '-fno-exceptions' ],
    'cflags_cc!': [ '-fno-exceptions' ],
    'cflags_cc': ['-std=c++17'],
    'xcode_settings': {
      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
      'CLANG_CXX_LIBRARY': 'libc++',
      'MACOSX_DEPLOYMENT_TARGET': '11',
      'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
      'OTHER_CFLAGS': [
          '-std=c++17',
          '-stdlib=libc++'
        ],
    },
    'msvs_settings': {
      'VCCLCompilerTool': {
        'ExceptionHandling': 1,
        'AdditionalOptions': [
          '-std:c++17'
        ]
      }
    },
  }]
}