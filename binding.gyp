{
  "targets": [
    {
      "target_name": "user_privileges",
      "sources": [ "src/user_privileges.cpp" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_EXPERIMENTAL" ],
      "xcode_settings": {
        "OTHER_CPLUSPLUSFLAGS": [ "-fno-exceptions" ]
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        }
      }
    }
  ]
}
