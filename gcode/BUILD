load("@rules_cc//cc:defs.bzl", "cc_binary")
cc_binary(
    name = "gcode_gen",
    srcs = ["Image2GCode.cc",],
    deps = [
        "@//third_party:opencv",
    ],
)
cc_binary(
    name = "gcode_uncompress",
    srcs = ["GCodeUncompress.cc",],
)