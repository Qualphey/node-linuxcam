{
    "targets": [
        {
          "target_name": "v4l2",
            "sources": [
                "v4l2.cc",
                "webcam.cpp"
            ],
          "cflags": ["-Wall", "-Wextra", "-pedantic"],
          "cflags_c": ["-std=c11", "-Wno-unused-parameter"],
          "cflags_cc": ["-std=c++11"],
          'cflags!': [ '-fno-exceptions' ],
          'cflags_cc!': [ '-fno-exceptions' ]
        }
    ]
}
