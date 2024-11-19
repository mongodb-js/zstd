{
  "targets": [
    {
      "target_name": "zstd",
      "type": "loadable_module",
      "defines": [
        "ZSTD_STATIC_LINKING_ONLY=true"
      ],
      "include_dirs": [
        "<!(node -p \"require(\"node-addon-api\").include_dir\")",
        "<(module_root_dir)/deps/zstd/lib"
      ],
      "variables": {
        "ARCH": "<(host_arch)",
        "built_with_electron%": 0
      },
      "sources": [
        "addon/zstd.cpp",
        "addon/compression_worker.h",
        "addon/compression.h",
        "addon/compression.cpp"
      ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "11",
        "GCC_SYMBOLS_PRIVATE_EXTERN": "YES",
        "OTHER_CFLAGS": [
          "-std=c++17",
          "-stdlib=libc++"
        ]
      },
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "cflags_cc": [
        "-std=c++17"
      ],
      "conditions": [
        [
          "OS=='win'",
          {
            "msvs_settings": {
              "VCCLCompilerTool": {
                "ExceptionHandling": 1,
                "AdditionalOptions": [
                  "-std:c++17"
                ]
              }
            },
            "include_dirs": [
              "<(module_root_dir)/deps/include"
            ],
            "link_settings": {
              "libraries": [
                "<(module_root_dir)/deps/zstd/build/cmake/lib/libzstd.lib"
              ]
            }
          }
        ]
      ],
      "link_settings": {
        "libraries": [
          "<(module_root_dir)/deps/zstd/build/cmake/lib/libzstd.a"
        ]
      }
    }
  ]
}