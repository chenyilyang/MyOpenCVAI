# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
licenses(["notice"])
package(default_visibility = ["//mediapipe:__subpackages__"])
cc_binary(
    name = "hello_mediapipe",
    srcs = ["hello_mediapipe.cc"],
    copts = [],
    deps = [
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/framework:calculator_graph",
        "//mediapipe/framework/port:logging",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)