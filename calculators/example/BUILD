# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "example",
    srcs = ["example.cc"],
    copts = [],
    deps = [
        "//mediapipe/framework:calculator_framework",
        ],
    visibility=["//visibility:public"],
    alwayslink = 1,
)